#include <components/colorManager.hpp>
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

auto getPlainColor() {
    AnimatedColorOption colorFg;
    ColorManager& colMan = ColorManager::getInstance();
    auto tx = colMan.get(ColorType::TEXT);
    colorFg.inactive = tx;
    colorFg.enabled = true;

    return colorFg;
};

int main() {
    auto screen = ScreenInteractive::Fullscreen();
    auto window = WindowComponent(APP_TITLE);
    int currentTab = 0;

    initializeConfig();

    Components availablePages;

    int vSelector = 2;

    auto menuWeatherCity = MenuComponent(
        {
            {"Sunny"},
            {"16 °C"},
            {"󰁜 10 - 12 km/h"},
            {" 10%"},
            {" 10%"},
        },
        getPlainColor(), std::nullopt, [&](const EntryState& state) {
            Element e = text(state.label + "   ");
            if (state.active && vSelector == 2) {
                e = text("• " + state.label + " ");
                e = e | bold;
            }
            return e;
        });

    auto backButtonMenu =
        MenuComponent({{"Back", [&] { currentTab = 0; }}}, getPlainColor(),
                      std::nullopt, [&](const EntryState& state) {
                          Element e =
                              hbox({text(state.label) | border, filler()});
                          if (state.active && vSelector == 0) {
                              e = e | bold | color(Color::Red);
                          }
                          return e;
                      });

    std::string small_sun_ascii = R"(
     .
   \ | /
 '-.;;;.-'
-==;;;;;==-
 .-';;;'-.
   / | \
     '
)";

    auto small_ascii_comp = Renderer([&small_sun_ascii] {
        std::string cur = "";
        std::vector<Element> v;
        for (auto c : small_sun_ascii) {
            if (c == '\n') {
                if (cur != "")
                    v.push_back(text(cur));
                cur = "";
            } else
                cur = cur + c;
        }
        return hbox({
            filler() | size(WIDTH, EQUAL, 1),
            vbox(v),
            filler() | size(WIDTH, EQUAL, 2),
        });
    });

    auto placeholderComp = menuWeatherCity.getComponent();

    auto cityComp = Renderer(placeholderComp, [&] {
        return hbox({
            small_ascii_comp->Render(),
            placeholderComp->Render() | center,
        });
    });

    auto cityWindow = WindowComponent("Bucharest");
    cityWindow.setChild(cityComp);
    cityComp = cityWindow.getComponent();

    auto cityPage = Container::Vertical(
        {
            backButtonMenu.getComponent(),
            Renderer([] { return filler(); }),
            Renderer(cityComp,
                     [&] {
                         return hbox({filler(), cityComp->Render(), filler()}) |
                                center;
                     }) |
                center,
            Renderer([] { return filler(); }),
        },
        &vSelector);

    auto mainMenu = MenuComponent({
        {"Do Nothing",
         [&] {
             currentTab = 1;
             vSelector = 2;
         }},
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

    availablePages = {mainPage, cityPage};
    auto pages = Container::Tab(availablePages, &currentTab);

    window.setChild(pages);
    auto mainComponent = window.getComponent();

    screen.Loop(mainComponent);

    quitAnimationThread = true;
    timerThread.join();

    return 0;
}
