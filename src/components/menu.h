#ifndef MENU_H
#define MENU_H

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>

enum class MenuButtons {
    Nothing = 0,
    Exit = 1,
};

class MenuComponent {
  public:
    MenuComponent(std::function<void(MenuButtons)> cb);
    ftxui::Component getComponent();

  private:
    std::vector<std::string> entries;
    int selected;
    ftxui::MenuOption option;
    std::function<void(MenuButtons)> callback;
};

#endif
