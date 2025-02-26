#include <colorManager.hpp>
#include <ftxui/dom/elements.hpp>
#include <menu.hpp>
#include <optional>
#include <weatherPage.hpp>
#include <weatherPageArt.hpp>
#include <window.hpp>

using namespace wPage;
using namespace ftxui;

std::unordered_map<Status, std::string> statusToString = {
    {Status::CLEAR, "Clear"},
    {Status::PARTLY_CLOUDY, "Part Cloudy"},
    {Status::CLOUDY, "Cloudy"},
    {Status::OVERCAST, "Overcast"},
    {Status::FOG, "Fog"},
    {Status::DRIZZLE, "Drizzle"},
    {Status::LIGHT_RAIN, "Light Rain"},
    {Status::RAIN, "Rain"},
    {Status::HEAVY_RAIN, "Heavy Rain"},
    {Status::LIGHT_SNOW, "Light Snow"},
    {Status::SNOW, "Snow"},
    {Status::HEAVY_SNOW, "Heavy Snow"},
    {Status::THUNDER, "Thunder"},
};

std::unordered_map<WindDir, std::string> windDirToString = {
    {WindDir::NORTH, "󰁝"}, {WindDir::NORTHEAST, "󰁜"},
    {WindDir::EAST, "󰁔"},  {WindDir::SOUTHEAST, "󰁃"},
    {WindDir::SOUTH, "󰁅"}, {WindDir::SOUTHWEST, "󰁂"},
    {WindDir::WEST, "󰁍"},  {WindDir::NORTHWEST, "󰁛"},
};

std::unordered_map<LabelName, std::string> LabelToName = {
    {LabelName::HUMIDITY, "HUMIDITY"},
    {LabelName::PRECIPITATION, "PRECIPITATION"},
    {LabelName::TEMPERATURE, "TEMPERATURE"},
    {LabelName::WIND_DATA, "WIND_DATA"},
    {LabelName::W_STATUS, "W_STATUS"},
};

std::string convertDouble(double d) {
    auto s = std::to_string(d);
    auto rounded = s.substr(0, s.find('.') + 2);
    return rounded;
}

std::string processWeatherStatus(wPage::Status wStatus) {
    return statusToString[wStatus];
}
std::string processTemperature(double temperature) {
    return convertDouble(temperature) + " °C";
}

std::string processWind(wPage::WindData windData) {
    return windDirToString[windData.dir] + " " +
           std::to_string(windData.speed) + " km/h";
}

std::string processHumidity(int humidity) {
    return " " + std::to_string(humidity) + "%";
}

std::string processPrecipitation(int precipitation) {
    return " " + std::to_string(precipitation) + "%";
}

MenuComponent* createBackButton(std::function<void()>, int&);

WeatherPage::WeatherPage(std::function<void()> backCallback) {
    vSelector = 0;

    backButton = createBackButton(backCallback, vSelector);
    menuWeatherCity = createMenuWeatherCity();
}

void WeatherPage::updateWeatherPage(std::string newCityName,
                                    wPage::Status newWStatus,
                                    double newTemperature,
                                    wPage::WindData newWindData,
                                    int newHumidity, int newPrecipitation) {

    cityName = newCityName;

    menuWeatherCity->updateEntry(LabelToName[W_STATUS],
                                 processWeatherStatus(newWStatus));
    art = weatherPageArt::statusToArt.at(newWStatus);

    menuWeatherCity->updateEntry(LabelToName[TEMPERATURE],
                                 processTemperature(newTemperature));
    menuWeatherCity->updateEntry(LabelToName[WIND_DATA],
                                 processWind(newWindData));
    menuWeatherCity->updateEntry(LabelToName[HUMIDITY],
                                 processHumidity(newHumidity));
    menuWeatherCity->updateEntry(LabelToName[PRECIPITATION],
                                 processPrecipitation(newPrecipitation));
}

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

    auto small_ascii_comp = Renderer([this] {
        std::string cur = "";
        std::vector<Element> v;
        for (auto c : art) {
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
            placeholderComp->Render() | size(WIDTH, EQUAL, 14) | center,
        });
    });

    vSelector = 0;

    auto cityPage = Container::Vertical(
        {
            backButton->getComponent(),
            Renderer([] { return filler(); }),
            // menuWeatherCity->getComponent(),
            Renderer(cityComp,
                     [&] {
                         auto weatherWindow = window(text("|" + cityName + "|"),
                                                     cityComp->Render());

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
            {
                processWeatherStatus(wStatus),
                [] {},
                LabelToName[LabelName::W_STATUS],
            },
            {
                processTemperature(temperature),
                [] {},
                LabelToName[LabelName::TEMPERATURE],
            },
            {
                processWind(windData),
                [] {},
                LabelToName[LabelName::WIND_DATA],
            },
            {
                processHumidity(humidity),
                [] {},
                LabelToName[LabelName::HUMIDITY],
            },
            {
                processPrecipitation(precipitation),
                [] {},
                LabelToName[LabelName::PRECIPITATION],
            },
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
