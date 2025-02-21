#ifndef WHEATHERPAGE_H
#define WHEATHERPAGE_H

#include <ftxui/component/component.hpp>
#include <menu.hpp>

namespace wPage {
enum Status {
    SUNNY,
};

enum WindDir {
    NORTHWEST,
    NORTH,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
};

struct WindData {
    WindDir dir;
    int minSpeed;
    int maxSpeed;
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
    void updateLabels(std::vector<wPage::WeatherEntry>);

  private:
    std::string cityName;
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
