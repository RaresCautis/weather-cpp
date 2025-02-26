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
        if (entry.labelName != "")
            labelEntriesMap[entry.labelName] = entries.labels.size() - 1;
    }
}

void MenuComponent::addEntry(menucomp::SingleMenuEntry newEntry, int index) {
    if (index > entries.labels.size())
        throw std::invalid_argument("addEntry index out of bounds");

    for (auto mapEntry : labelEntriesMap) {
        if (mapEntry.second >= index)
            labelEntriesMap[mapEntry.first]++;
    }

    entries.labels.insert(entries.labels.begin() + index, newEntry.label);
    entries.callbacks.insert(entries.callbacks.begin() + index,
                             newEntry.callback);
    if (newEntry.labelName != "")
        labelEntriesMap[newEntry.labelName] = index;
}

void MenuComponent::updateEntry(std::string labelName, std::string newLabel) {
    auto index = labelEntriesMap[labelName];
    entries.labels[index] = newLabel;
}

void MenuComponent::updateLabels(std::vector<string>& newLabels,
                                 bool resetSelected) {
    entries.labels.clear();
    for (auto label : newLabels) {
        entries.labels.push_back(label);
    }

    if (resetSelected)
        selected = 0;
}

int MenuComponent::getSelected() { return selected; }

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
