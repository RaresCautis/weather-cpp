#include "menu.h"

using namespace ftxui;

MenuComponent::MenuComponent(std::function<void(MenuButtons)> cb) {
    callback = cb;
    selected = 0;
    entries = {
        "Do Nothing",
        "Exit",
    };

    auto onMenuSelect = [&] {
        MenuButtons selectedButton = static_cast<MenuButtons>(selected);
        callback(selectedButton);
    };

    option.on_enter = onMenuSelect;
}

Component MenuComponent::getComponent() {
    auto menu = Menu(&entries, &selected, option);
    return menu;
}
