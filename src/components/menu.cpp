#include "menu.hpp"
#include "helpers.hpp"
#include <chrono>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>

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
    option.focused_entry = &selected;

    AnimatedColorOption colorFg;
    auto tx = Color::RGB(205, 214, 244);
    auto curs = Color::RGB(250, 179, 135);
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

Component MenuComponent::getComponent() {
    auto menu = Menu(&entries, &selected, option);

    return makeClickableComponent(menu);
}
