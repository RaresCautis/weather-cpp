#ifndef WHEATHERPAGE_H
#define WHEATHERPAGE_H

#include <ftxui/component/component.hpp>
#include <menu.hpp>
#include <window.hpp>

namespace wPage {
enum Status {
    CLEAR,
    PARTLY_CLOUDY,
    CLOUDY,
    OVERCAST,
    FOG,
    DRIZZLE,
    LIGHT_RAIN,
    RAIN,
    HEAVY_RAIN,
    LIGHT_SNOW,
    SNOW,
    HEAVY_SNOW,
    THUNDER,
};

enum WindDir {
    NORTH,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
    NORTHWEST,
};

struct WindData {
    WindDir dir;
    int speed;
};

enum LabelName {
    W_STATUS,
    TEMPERATURE,
    WIND_DATA,
    HUMIDITY,
    PRECIPITATION,
};

struct WeatherEntry {
    LabelName name;
    std::string value;
};

}; // namespace wPage

class WeatherPage {
  public:
    WeatherPage(std::string, wPage::Status, double, wPage::WindData, int, int,
                std::function<void()>);
    ~WeatherPage();
    ftxui::Component getComponent();
    void updateWeatherPage(std::string, wPage::Status, double, wPage::WindData,
                           int, int);
    void updateLabels(std::vector<wPage::WeatherEntry>);

  private:
    std::string cityName;
    std::string art;
    wPage::Status wStatus;
    double temperature;
    wPage::WindData windData;
    int humidity;
    int precipitation;
    MenuComponent* backButton;
    MenuComponent* menuWeatherCity;
    ftxui::Component cityComp;
    int vSelector;
    std::thread stupid;

    MenuComponent* createMenuWeatherCity();
};

#endif
