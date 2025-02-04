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

}; // namespace wPage

class WeatherPage {
  public:
    WeatherPage(std::string, wPage::Status, double, wPage::WindData, int, int,
                std::function<void()>);
    ~WeatherPage();
    ftxui::Component getComponent();

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
