#include <serializers.hpp>

using namespace std;
using namespace nlohmann;
using namespace ftxui;

namespace {

ftxui::Color getColorFromHex(string hex) {
    string redHex = hex.substr(0, 2);
    string greenHex = hex.substr(2, 2);
    string blueHex = hex.substr(4, 2);
    int red = stoi(redHex, 0, 16);
    int green = stoi(greenHex, 0, 16);
    int blue = stoi(blueHex, 0, 16);

    return Color::RGB(red, green, blue);
}

}; // namespace

serializers::colorArray
nlohmann::adl_serializer<serializers::colorArray>::from_json(
    const nlohmann::json& colList) {
    serializers::colorArray newColorMap;

    for (auto color : colList) {
        std::string name = color.at("name");
        auto type = stringToColorType(name);

        if (!type)
            continue;

        std::string value = color.at("value");

        newColorMap.push_back({
            *type,
            getColorFromHex(value),
        });
    }

    return newColorMap;
}

std::unordered_map<int, wPage::Status> weatherStatusMap = {
    {0, wPage::Status::CLEAR},       {1, wPage::Status::PARTLY_CLOUDY},
    {2, wPage::Status::CLOUDY},      {3, wPage::Status::OVERCAST},
    {45, wPage::Status::FOG},        {48, wPage::Status::FOG},
    {51, wPage::Status::DRIZZLE},    {53, wPage::Status::DRIZZLE},
    {55, wPage::Status::DRIZZLE},    {56, wPage::Status::DRIZZLE},
    {57, wPage::Status::DRIZZLE},    {61, wPage::Status::LIGHT_RAIN},
    {63, wPage::Status::RAIN},       {65, wPage::Status::HEAVY_RAIN},
    {66, wPage::Status::RAIN},       {67, wPage::Status::HEAVY_RAIN},
    {71, wPage::Status::LIGHT_SNOW}, {73, wPage::Status::SNOW},
    {75, wPage::Status::HEAVY_SNOW}, {77, wPage::Status::LIGHT_SNOW},
    {80, wPage::Status::LIGHT_RAIN}, {81, wPage::Status::RAIN},
    {82, wPage::Status::HEAVY_RAIN}, {85, wPage::Status::RAIN},
    {86, wPage::Status::HEAVY_RAIN}, {95, wPage::Status::THUNDER},
    {96, wPage::Status::THUNDER},    {99, wPage::Status::THUNDER},
};

vector<wPage::WindDir> directions = {
    wPage::WindDir::NORTH, wPage::WindDir::NORTHEAST,
    wPage::WindDir::EAST,  wPage::WindDir::SOUTHEAST,
    wPage::WindDir::SOUTH, wPage::WindDir::SOUTHWEST,
    wPage::WindDir::WEST,  wPage::WindDir::NORTHWEST,
};

wPage::WindDir getWindDir(int deg) { return directions[deg / 45]; }

int getPrecipitation(time_t current_time, nlohmann::json time,
                     nlohmann::json precipitation) {
    int hour = 23;
    while (time[hour] > current_time)
        hour--;

    int max_precipitation = 0;

    for (int i = hour; i < 24 && i - hour < 3; i++) {
        int i_precipitation = precipitation[i];
        max_precipitation = max(max_precipitation, i_precipitation);
    }

    return max_precipitation;
}

weatherFetcher::weatherData
nlohmann::adl_serializer<weatherFetcher::weatherData>::from_json(
    const nlohmann::json& weatherJson) {
    weatherFetcher::weatherData data;

    auto current = weatherJson.at("current");

    auto wCode = current.at("weather_code");
    data.weatherCode = weatherStatusMap[wCode];

    auto temp = current.at("temperature_2m");
    data.temperature = temp;

    auto windDir = getWindDir(current.at("wind_direction_10m"));
    auto windSpeed = current.at("wind_speed_10m");
    data.wind = {windDir, windSpeed};

    auto humidity = current.at("relative_humidity_2m");
    data.humidity = humidity;

    auto precipitation = getPrecipitation(
        current.at("time"), weatherJson.at("hourly").at("time"),
        weatherJson.at("hourly").at("precipitation_probability"));
    data.precipitation = precipitation;

    return data;
}
