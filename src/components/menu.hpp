#ifndef MENU_H
#define MENU_H

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>

namespace menucomp {

using menuCallback = std::function<void()>;

struct SingleMenuEntry {
    std::string label;
    menuCallback callback;
};

struct MenuEntries {
    std::vector<std::string> labels;
    std::vector<menuCallback> callbacks;
};

}; // namespace menucomp

class MenuComponent {
  public:
    MenuComponent(std::vector<menucomp::SingleMenuEntry> newEntries);
    ftxui::Component getComponent();

  private:
    void addEntries(std::vector<menucomp::SingleMenuEntry>& newEntries);
    void setupOptions();
    menucomp::MenuEntries entries;
    int selected;
    ftxui::MenuOption option;
};

#endif
