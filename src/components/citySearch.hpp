#ifndef CITYSEARCH_H
#define CITYSEARCH_H

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <menu.hpp>

namespace citySearch {

struct CityData {
    std::string name;
    std::string country;
    double latitude;
    double longitude;
};

}; // namespace citySearch

class CitySearch {
  public:
    CitySearch(citySearch::CityData*, std::function<void()>);
    void toggleVisible();
    ftxui::Component getComponent();
    ~CitySearch();

  private:
    citySearch::CityData* returnCity;
    std::vector<citySearch::CityData> foundCities;
    ftxui::Component citySearchComponent;
    ftxui::Component searchInputComponent;
    ftxui::Component inputBox;
    MenuComponent* searchResultsMenu;
    ftxui::Component searchResultsMenuComponent;
    ftxui::Component searchResults;
    ftxui::Component containerV;
    int verticalSelector = 0;
    bool isVisible = false;
    std::function<void()> changeFocus;
    ftxui::Color textCol;

    void createSearchInput();
    void createSearchResults();
    void createCitySearch();
};

#endif
