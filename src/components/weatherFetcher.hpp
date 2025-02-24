#ifndef WEATHERFETCHER_H
#define WEATHERFETCHER_H

#include <weatherPage.hpp>

namespace weatherFetcher {
struct weatherData {
    wPage::Status weatherCode;
    double temperature;
    wPage::WindData wind;
    int humidity;
    int precipitation;
};

weatherData fetchWeather(double, double);
}; // namespace weatherFetcher

#endif
