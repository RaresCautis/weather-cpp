#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <serializers.hpp>
#include <weatherFetcher.hpp>

#define URL "https://api.open-meteo.com/v1/forecast"
#define CURRENT                                                                \
    "temperature_2m,relative_humidity_2m,weather_code,wind_speed_10m,wind_"    \
    "direction_10m"
#define HOURLY "precipitation_probability"
#define TIMEFORMAT "unixtime"
#define TIMEZONE "GMT"
#define FORECAST_DAYS "1"

using namespace weatherFetcher;

weatherData weatherFetcher::fetchWeather(double latitude, double longitude) {
    auto response =
        cpr::Get(cpr::Url{URL}, cpr::Parameters{
                                    {"latitude", std::to_string(latitude)},
                                    {"longitude", std::to_string(longitude)},
                                    {"current", CURRENT},
                                    {"hourly", HOURLY},
                                    {"timeformat", TIMEFORMAT},
                                    {"timezone", TIMEZONE},
                                    {"forecast_days", FORECAST_DAYS},
                                });
    auto json = nlohmann::json::parse(response.text);

    return json.template get<weatherData>();
}
