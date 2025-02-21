#include <colorManager.hpp>
#include <menu.hpp>
#include <optional>
#include <weatherPage.hpp>
#include <window.hpp>

using namespace wPage;
using namespace ftxui;

MenuComponent* createBackButton(std::function<void()>, int&);

WeatherPage::WeatherPage(std::string newCityName, wPage::Status newWStatus,
                         double newTemperature, wPage::WindData newWindData,
                         int newHumidity, int newPrecipitation,
                         std::function<void()> backCallback) {
    cityName = newCityName;
    wStatus = newWStatus;
    temperature = newTemperature;
    windData = newWindData;
    humidity = newHumidity;
    precipitation = newPrecipitation;
    vSelector = 0;

    backButton = createBackButton(backCallback, vSelector);
    menuWeatherCity = createMenuWeatherCity();
}

std::unordered_map<Status, std::string> statusToString = {
    {Status::SUNNY, "Sunny"},
};

std::unordered_map<WindDir, std::string> windDirToString = {
    {WindDir::NORTHWEST, "󰁛"}, {WindDir::NORTH, "󰁝"},
    {WindDir::NORTHEAST, "󰁜"}, {WindDir::EAST, "󰁔"},
    {WindDir::SOUTHEAST, "󰁃"}, {WindDir::SOUTH, "󰁅"},
    {WindDir::SOUTHWEST, "󰁂"}, {WindDir::WEST, "󰁍"},
};

std::unordered_map<LabelName, std::string> LabelToName = {
    {LabelName::HUMIDITY, "HUMIDITY"},
    {LabelName::PRECIPITATION, "PRECIPITATION"},
    {LabelName::TEMPERATURE, "TEMPERATURE"},
    {LabelName::WIND_DATA, "WIND_DATA"},
    {LabelName::W_STATUS, "W_STATUS"},
};

auto getPlainColor() {
    AnimatedColorOption colorFg;
    ColorManager& colMan = ColorManager::getInstance();
    auto tx = colMan.get(ColorType::TEXT);
    colorFg.inactive = tx;
    colorFg.enabled = true;

    return colorFg;
};

WeatherPage::~WeatherPage() {
    delete (backButton);
    delete (menuWeatherCity);
}

Component WeatherPage::getComponent() {

    std::string small_sun_ascii = R"(
     .
   \ | /
 '-.;;;.-'
-==;;;;;==-
 .-';;;'-.
   / | \
     '
)";

    auto small_ascii_comp = Renderer([small_sun_ascii] {
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

    auto placeholderComp = menuWeatherCity->getComponent();

    cityComp = Renderer(placeholderComp, [placeholderComp, small_ascii_comp] {
        return hbox({
            small_ascii_comp->Render(),
            placeholderComp->Render() | center,
        });
    });

    auto cityWindow = WindowComponent(cityName);
    cityWindow.setChild(cityComp);

    vSelector = 0;

    auto cityPage = Container::Vertical(
        {
            backButton->getComponent(),
            Renderer([] { return filler(); }),
            // menuWeatherCity->getComponent(),
            Renderer(cityComp,
                     [&] {
                         auto weatherWindow =
                             window(text(cityName), cityComp->Render());

                         return hbox({
                                    filler(),
                                    weatherWindow,
                                    filler(),
                                }) |
                                center;
                     }) |
                center,
            Renderer([] { return filler(); }),
        },
        &vSelector);

    return cityPage;
}

void WeatherPage::updateLabels(
    std::vector<wPage::WeatherEntry> updatedEntries) {
    for (auto entry : updatedEntries) {
        menuWeatherCity->updateEntry(LabelToName[entry.name], entry.value);
    }
}

MenuComponent* createBackButton(std::function<void()> backCallback,
                                int& vSelector) {
    return new MenuComponent(
        {{"Back", [backCallback] { backCallback(); }}}, getPlainColor(),
        std::nullopt, [&](const EntryState& state) {
            Element e = hbox({text(state.label) | border, filler()});
            if (state.active && vSelector == 0) {
                e = e | bold | color(Color::Red);
            }
            return e;
        });
}

MenuComponent* WeatherPage::createMenuWeatherCity() {
    return new MenuComponent(
        {
            {statusToString[wStatus], [] {}, LabelToName[LabelName::W_STATUS]},
            {std::to_string(temperature) + " °C", [] {},
             LabelToName[LabelName::TEMPERATURE]},
            {windDirToString[windData.dir] + " " +
                 std::to_string(windData.minSpeed) + " - " +
                 std::to_string(windData.maxSpeed) + " km/h",
             [] {}, LabelToName[LabelName::WIND_DATA]},
            {" " + std::to_string(humidity) + "%", [] {},
             LabelToName[LabelName::HUMIDITY]},
            {" " + std::to_string(precipitation) + "%", [] {},
             LabelToName[LabelName::PRECIPITATION]},
        },
        getPlainColor(), std::nullopt, [this](const EntryState& state) {
            Element e = text(state.label + "   ");
            if (state.active && vSelector == 2) {
                e = text("• " + state.label + " ");
                e = e | bold;
            }
            return e;
        });
};
