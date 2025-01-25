#include "menu.hpp"
#include "colorManager.hpp"
#include "helpers.hpp"

using namespace menucomp;
using namespace ftxui;

MenuComponent::MenuComponent(std::vector<SingleMenuEntry> newEntries) {
    addEntries(newEntries);
    setupOptions();
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

void MenuComponent::setupOptions() {
    auto onMenuSelect = [this] { entries.callbacks[selected](); };
    option.on_enter = onMenuSelect;

    selected = 0;
    option.focused_entry = &selected;

    AnimatedColorOption colorFg;
    ColorManager& colMan = ColorManager::getInstance();
    auto tx = colMan.get(ColorType::TEXT);
    auto curs = colMan.get(ColorType::HIGHLIGHT);
    colorFg.active = curs;
    colorFg.inactive = tx;
    colorFg.enabled = true;
    colorFg.duration = std::chrono::milliseconds(100);
    AnimatedColorOption colorBg;
    AnimatedColorsOption optionColor = {colorBg, colorFg};
    MenuEntryOption menuOption;
    menuOption.animated_colors = optionColor;

    option.entries_option = menuOption;
    option.entries_option.transform = [](const EntryState& state) {
        Element e = text(state.label) | center;
        /*auto grad = LinearGradient(90, Color::Red, Color::Blue);
        double size = state.label.size();
        double position = 1 / (3 / size);
        grad.Stop(Color::Blue, 5);*/
        if (state.active) {
            e = e | bold;
            // e = e | color(grad);
        }
        return e;
    };
}
