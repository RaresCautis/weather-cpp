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

  private:
    void addEntries(std::vector<menucomp::SingleMenuEntry>& newEntries);
    void setupOptions(
        std::optional<ftxui::AnimatedColorOption> colorFg = std::nullopt,
        std::optional<ftxui::AnimatedColorOption> colorBg = std::nullopt,
        std::optional<menucomp::transformFunc> transformFunc = std::nullopt);
    menucomp::MenuEntries entries;
    int selected;
    ftxui::MenuOption option;
};

#endif
