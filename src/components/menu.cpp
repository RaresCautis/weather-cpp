#include <colorManager.hpp>
#include <helpers.hpp>
#include <menu.hpp>

using namespace menucomp;
using namespace ftxui;
using namespace std;

MenuComponent::MenuComponent(vector<SingleMenuEntry> newEntries,
                             optional<AnimatedColorOption> colorFg,
                             optional<AnimatedColorOption> colorBg,
                             optional<transformFunc> transformFunc) {

    addEntries(newEntries);
    setupOptions(colorFg, colorBg, transformFunc);
}

Component MenuComponent::getComponent() {
    auto menu = Menu(&entries.labels, &selected, option);

    return makeClickableComponent(menu);
}

void MenuComponent::addEntries(std::vector<SingleMenuEntry>& newEntries) {
    for (auto entry : newEntries) {
        entries.labels.push_back(entry.label);
        entries.callbacks.push_back(entry.callback);
    }
}

AnimatedColorOption getDefaultColorFg();
AnimatedColorOption getDefaultColorBg();
transformFunc getDefaultTransformFunc();

void MenuComponent::setupOptions(optional<AnimatedColorOption> colorFg,
                                 optional<AnimatedColorOption> colorBg,
                                 optional<transformFunc> transformFunc) {
    auto onMenuSelect = [this] { entries.callbacks[selected](); };
    option.on_enter = onMenuSelect;

    selected = 0;
    option.focused_entry = &selected;

    AnimatedColorsOption optionColor = {
        colorBg.value_or(getDefaultColorBg()),
        colorFg.value_or(getDefaultColorFg()),
    };

    MenuEntryOption menuOption;
    menuOption.animated_colors = optionColor;

    option.entries_option = menuOption;
    option.entries_option.transform =
        transformFunc.value_or(getDefaultTransformFunc());
}

AnimatedColorOption getDefaultColorFg() {
    AnimatedColorOption colorFg;
    ColorManager& colMan = ColorManager::getInstance();
    auto tx = colMan.get(ColorType::TEXT);
    auto curs = colMan.get(ColorType::HIGHLIGHT);
    colorFg.active = curs;
    colorFg.inactive = tx;
    colorFg.enabled = true;
    colorFg.duration = std::chrono::milliseconds(100);

    return colorFg;
}

AnimatedColorOption getDefaultColorBg() {
    AnimatedColorOption colorBg;

    return colorBg;
}

transformFunc getDefaultTransformFunc() {
    return [](const EntryState& state) {
        Element e = text(state.label) | center;
        if (state.active) {
            e = e | bold;
        }
        return e;
    };
}
