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
    CitySearch(std::function<void(citySearch::CityData)>,
               std::function<void()>);
    void toggleVisible();
    ftxui::Component getComponent();
    void resetComponent();
    ~CitySearch();

  private:
    std::function<void(citySearch::CityData)>
        selectCallback; // Need to remove some of the attrs
    std::vector<citySearch::CityData> foundCities;
    ftxui::Component citySearchComponent;
    ftxui::Component searchInputComponent;
    ftxui::Component inputBox;
    std::string searchContent;
    std::atomic<bool> isSearching;
    std::atomic<time_t> searchTime;
    MenuComponent* searchResultsMenu;
    ftxui::Component searchResultsMenuComponent;
    ftxui::Component searchResults;
    ftxui::Component containerV;
    int verticalSelector = 0;
    bool isVisible = false;
    std::function<void()> changeFocus;
    ftxui::Color textCol;
    std::thread searchThread;

    void createSearchInput();
    void createSearchResults();
    void createCitySearch();
    void startSearchThread();
};

#endif
