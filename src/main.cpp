#include "components/menu.hpp"
#include "components/window.hpp"
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <thread>

#define APP_TITLE "weather-cpp"

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::Fullscreen();
    auto window = WindowComponent(APP_TITLE);

    int currentTab = 0;

    auto menuCallback = [&](MenuButtons buttonPressed) {
        switch (buttonPressed) {
        case MenuButtons::Exit:
            screen.Exit();
            break;
        case MenuButtons::Nothing:
            break;
        }
    };

    auto mainMenu = MenuComponent(menuCallback);

    std::string ascii_art = R"(
                |
                |
      `.        |        .'
        `.    .---.    .'
           .~       ~.
          /   O   O   \
-- -- -- (             ) -- -- --
          \    `-'    /
           ~.       .~
        .'    ~---~    `.
      .'        |        `.
                |
                |
    )";

    std::string ascii_art2 = R"(
               |
                 |
       `.      |        .'
        `.    .---.    .'
           .~       ~.
--    --  /   O   O   \
   --    (             ) --    --
          \    `-'    /     --
           ~.       .~
        .'    ~---~    `.
       .'        |      `.
               |
                 |
    )";

    std::string aux;

    std::atomic<bool> quitAnimationThread = false;
    std::thread timerThread(
        [&ascii_art, &ascii_art2, &aux, &screen, &quitAnimationThread] {
            while (!quitAnimationThread) {
                aux = ascii_art;
                ascii_art = ascii_art2;
                ascii_art2 = aux;

                screen.PostEvent(Event::Custom);

                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });

    auto ascii_comp = Renderer([&ascii_art] {
        std::string cur = "";
        std::vector<Element> v;
        for (auto c : ascii_art) {
            if (c == '\n') {
                v.push_back(text(cur));
                cur = "";
            } else
                cur = cur + c;
        }
        return vbox(v);
    });

    auto comp = mainMenu.getComponent();

    auto component = Renderer(comp, [&] {
        return vbox({
            ascii_comp->Render(),
            comp->Render() | center,
        });
    });

    // Components availablePages = {mainMenu.getComponent()};
    Components availablePages = {component};
    auto mainPages = Container::Tab(availablePages, &currentTab);

    window.setComponent(ComponentPosition::center, mainPages);
    auto mainComponent = window.getWindow();

    screen.Loop(mainComponent);

    quitAnimationThread = true;
    timerThread.join();

    return 0;
}
