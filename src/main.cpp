#include <components/config.hpp>
#include <components/menu.hpp>
#include <components/weatherFetcher.hpp>
#include <components/weatherPage.hpp>
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
    int currentTab = 0;

    initializeConfig();

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

    auto mainPage = Renderer(comp, [&] {
        return vbox({
            filler(),
            stupidAnimationComponent->Render() | center,
            comp->Render() | center,
            filler(),
        });
    });

    auto j = weatherFetcher::fetchWeather(51.52, -0.08000016); // move

    auto weatherPage =
        WeatherPage("London", j.weatherCode, j.temperature, j.wind, j.humidity,
                    j.precipitation, [&] { currentTab = 0; }); // move
    auto cityPage = weatherPage.getComponent();

    auto availablePages = {mainPage, cityPage};
    auto pages = Container::Tab(availablePages, &currentTab);

    window.setChild(pages);
    auto mainComponent = window.getComponent();

    screen.Loop(mainComponent);

    quitAnimationThread = true;
    timerThread.join();

    return 0;
}
