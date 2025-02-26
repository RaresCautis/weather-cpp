#include <components/citySearch.hpp>
#include <components/config.hpp>
#include <components/menu.hpp>
#include <components/weatherFetcher.hpp>
#include <components/weatherPage.hpp>
#include <components/window.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <thread>

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::Fullscreen();
    screen.TrackMouse(false);
    auto mainWindow = WindowComponent(APP_TITLE);
    int currentTab = 0;
    Component mainComponent;

    initializeConfig();

    MenuComponent* mainMenuPointer;
    WeatherPage* weatherPagePointer;

    std::function<void(citySearch::CityData)> citySearchCallback =
        [&mainMenuPointer, &currentTab,
         &weatherPagePointer](citySearch::CityData city) {
            mainMenuPointer->addEntry(
                {city.name,
                 [mainMenuPointer, city, weatherPagePointer, &currentTab] {
                     auto j = weatherFetcher::fetchWeather(
                         city.latitude, city.longitude); // move

                     weatherPagePointer->updateWeatherPage(
                         city.name, j.weatherCode, j.temperature, j.wind,
                         j.humidity, j.precipitation);

                     currentTab = 1;
                 }},
                0);
        };
    ;

    CitySearch citySearch = CitySearch(
        citySearchCallback, [&mainComponent] { mainComponent->TakeFocus(); });
    auto citySearchComponent = citySearch.getComponent();

    auto mainMenu = MenuComponent({
        {"Add Location",
         [&citySearch] {
             citySearch.resetComponent();
             citySearch.toggleVisible();
         }},
        {"Exit", [&screen] { screen.Exit(); }},
    });

    mainMenuPointer = &mainMenu;

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

    auto weatherPage = WeatherPage([&] { currentTab = 0; });
    auto cityPage = weatherPage.getComponent();
    weatherPagePointer = &weatherPage;

    auto availablePages = {mainPage, cityPage};
    auto pages = Container::Tab(availablePages, &currentTab);

    mainWindow.setChild(pages);
    mainComponent = mainWindow.getComponent();

    auto stackedApp =
        Container::Stacked({citySearchComponent, mainComponent}); // fix this
    mainComponent->TakeFocus();

    screen.Loop(stackedApp);

    quitAnimationThread = true;
    timerThread.join();

    return 0;
}
