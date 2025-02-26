#ifndef MENU_H
#define MENU_H

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <optional>

namespace menucomp {

using menuCallback = std::function<void()>;
using transformFunc = std::function<ftxui::Element(const ftxui::EntryState&)>;

struct SingleMenuEntry {
    std::string label;
    menuCallback callback = [] {};
    std::string labelName = "";
};

struct MenuEntries {
    std::vector<std::string> labels;
    std::vector<menuCallback> callbacks;
};

}; // namespace menucomp

class MenuComponent {
  public:
    MenuComponent(
        std::vector<menucomp::SingleMenuEntry> newEntries,
        std::optional<ftxui::AnimatedColorOption> colorFg = std::nullopt,
        std::optional<ftxui::AnimatedColorOption> colorBg = std::nullopt,
        std::optional<menucomp::transformFunc> transformFunc = std::nullopt);
    ftxui::Component getComponent();
    void updateEntry(std::string labelName, std::string newLabel);
    void updateLabels(std::vector<std::string>&, bool);

  private:
    void addEntries(std::vector<menucomp::SingleMenuEntry>& newEntries);
    void setupOptions(
        std::optional<ftxui::AnimatedColorOption> colorFg = std::nullopt,
        std::optional<ftxui::AnimatedColorOption> colorBg = std::nullopt,
        std::optional<menucomp::transformFunc> transformFunc = std::nullopt);
    menucomp::MenuEntries entries;
    int selected;
    ftxui::MenuOption option;
    // map from labelName to index in entries, not good
    std::unordered_map<std::string, int> labelEntriesMap;
};

#endif
