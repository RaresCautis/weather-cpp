#include <components/config.hpp>
#include <components/menu.hpp>
#include <components/window.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <thread>

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::Fullscreen();
    auto window = WindowComponent(APP_TITLE);

    initializeConfig();

    Components availablePages;

    auto menuTry = MenuComponent({
        {"asd", [] {}},
        {"asd", [] {}},
        {"asd", [] {}},
    });

    auto windowTry = WindowComponent("Bucharest");
    auto compc = menuTry.getComponent();
    windowTry.setComponent(ComponentPosition::center, compc);

    int currentTab = 0;
    auto mainMenu = MenuComponent({
        {"Do Nothing", [&] { currentTab = 1; }},
        {"Exit", [&screen] { screen.Exit(); }},
    });
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

    auto stupidAnimationComponent = Renderer([&ascii_art] {
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

    availablePages = {component, windowTry.getWindow()};
    auto mainPages = Container::Tab(availablePages, &currentTab);

    window.setChild(ComponentPosition::top, stupidAnimationComponent);
    window.setChild(ComponentPosition::center, mainPages);
    auto mainComponent = window.getComponent();

    screen.Loop(mainComponent);

    quitAnimationThread = true;
    timerThread.join();

    return 0;
}
