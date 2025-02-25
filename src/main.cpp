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
    auto tabtabsel = false;
    Component mainComponent;

    auto vfs = 0;

    std::vector<std::string> entries = {
        "HELLO", "HELLO", "HELLO", "HELLO", "HELLO", "HELLO", "HELLO", "HELLO",
    };

    auto btssselect = 0;
    auto mmmm = Menu(&entries, &btssselect);
    auto btsss = Renderer(mmmm, [&mmmm] {
        return mmmm->Render() | vscroll_indicator | yframe |
               size(ftxui::HEIGHT, ftxui::LESS_THAN, 3);
    });

    auto inasdt = Container::Vertical({Input() | CatchEvent([&vfs](Event e) {
                                           if (e == Event::ArrowDown ||
                                               e == Event::ArrowUp)
                                               return true;
                                           if (e == Event::Return) {
                                               vfs = 1;
                                               return true;
                                           }
                                           return false;
                                       })});

    auto bts = Container::Vertical(
        {
            inasdt,
            btsss,
        },
        &vfs);

    auto tr =
        Container::Horizontal({
            Renderer(bts,
                     [&bts] {
                         auto box = vbox({
                                        text("WORLD"),
                                        text("WORLD"),
                                        text("WORLD"),
                                        text("WORLD"),
                                        text("WORLD"),
                                        text("WORLD"),
                                        text("WORLD"),
                                        text("WORLD"),
                                        text("WORLD"),
                                    }) |
                                    size(ftxui::HEIGHT, ftxui::EQUAL, 1) |
                                    frame;

                         return vbox({filler(), window(text(""), bts->Render()),
                                      filler()}) |
                                center;
                     }),
        }) |
        center;

    auto tabtab = Maybe(tr, &tabtabsel);

    initializeConfig();

    auto mainMenu = MenuComponent({{"Do Nothing", [&] { currentTab = 1; }},
                                   {"Exit", [&screen] { screen.Exit(); }},
                                   {"HAHA", [&tabtabsel, &tabtab] {
                                        tabtabsel = 1;
                                        tabtab->TakeFocus();
                                    }}});
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
    std::thread timerThread([&ascii_art, &ascii_art2, &aux, &screen,
                             &quitAnimationThread, &entries] {
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

    // auto j = weatherFetcher::fetchWeather(51.52, -0.08000016); // move
    weatherFetcher::weatherData j;

    auto weatherPage =
        WeatherPage("London", j.weatherCode, j.temperature, j.wind, j.humidity,
                    j.precipitation, [&] { currentTab = 0; }); // move
    auto cityPage = weatherPage.getComponent();

    auto availablePages = {mainPage, cityPage};
    auto pages = Container::Tab(availablePages, &currentTab);

    mainWindow.setChild(pages);
    mainComponent = mainWindow.getComponent();

    auto xd = Container::Stacked({tabtab, mainComponent});
    xd->SetActiveChild(mainComponent);
    mainComponent->TakeFocus();

    screen.Loop(xd);

    quitAnimationThread = true;
    timerThread.join();

    return 0;
}
