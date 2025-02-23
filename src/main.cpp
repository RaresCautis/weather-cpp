#include <components/config.hpp>
#include <components/menu.hpp>
#include <components/weatherPage.hpp>
#include <components/window.hpp>
#include <cpr/cpr.h> // move
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp> // move
#include <string>
#include <thread>

using namespace ftxui;

nlohmann::basic_json<> getLondonWeather() { // move
    auto x = cpr::Get(
        cpr::Url{"https://api.open-meteo.com/v1/forecast"},
        cpr::Parameters{
            {"latitude", "51.5146"},
            {"longitude", "-0.0761"},
            {"current", "temperature_2m,relative_humidity_2m,precipitation,"
                        "weather_code,wind_speed_10m,wind_direction_10m"},
            {"timeformat", "unixtime"},
        });

    return nlohmann::json::parse(x.text);
}

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

    auto j = getLondonWeather()["current"]; // move

    auto weatherPage = WeatherPage(
        "London", wPage::Status::SUNNY, j["temperature_2m"],
        wPage::WindData({wPage::WindDir::NORTHEAST, j["wind_speed_10m"], 12}),
        j["relative_humidity_2m"], j["precipitation"], [&] { currentTab = 0; });
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
