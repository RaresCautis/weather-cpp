#include "components/menu.h"
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>

#define APP_TITLE "weather-cpp"

using namespace ftxui;

Component wrapInWindow(const std::string title, Component component) {
    std::string wrappedTitle = "|" + title + "|";

    return Renderer(component, [wrappedTitle, component] {
        return window(text(wrappedTitle), component->Render());
    });
}

int main() {
    auto screen = ScreenInteractive::Fullscreen();

    int currentTab = 0;

    auto menuCallback = [&](MenuButtons buttonPressed) {
        switch (buttonPressed) {
        case MenuButtons::Exit:
            screen.Exit();
            break;
        case MenuButtons::Nothing:
            currentTab = 1;
            break;
        }
    };

    auto mainMenu = MenuComponent(menuCallback);

    Components availablePages = {mainMenu.getComponent(),
                                 Container::Vertical({})};
    auto mainPages = Container::Tab(availablePages, &currentTab);

    auto mainComponent = wrapInWindow(APP_TITLE, mainPages);

    screen.Loop(mainComponent);
}
