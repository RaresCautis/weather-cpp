#include <chrono>
#include <citySearch.hpp>
#include <colorManager.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/screen/color.hpp>
#include <menu.hpp>
#include <optional>
#include <weatherFetcher.hpp>

using namespace ftxui;
void updateSearchTime(std::atomic<time_t>& sTime) {
    sTime = std::time(nullptr) + 1;
}

CitySearch::CitySearch(
    std::function<void(citySearch::CityData)> newSelectCallback,
    std::function<void()> newChangeFocus) {

    selectCallback = newSelectCallback;
    changeFocus = newChangeFocus;

    ColorManager& colMan = ColorManager::getInstance();
    textCol = colMan.get(ColorType::TEXT);

    searchContent = "";

    isSearching = false;
    searchTime = std::time(nullptr);

    createSearchResults();
    createSearchInput();
    createCitySearch();
}

void CitySearch::toggleVisible() {
    if (isVisible) {
        isVisible = false;
        changeFocus();
    } else {
        isVisible = true;
        citySearchComponent->TakeFocus();
    }
}

Component CitySearch::getComponent() { return citySearchComponent; }

void CitySearch::resetComponent() {
    std::vector<std::string> emptyStringVector;
    foundCities.clear();
    searchResultsMenu->updateLabels(emptyStringVector, true);
    searchContent = "";
    verticalSelector = 0;
}

CitySearch::~CitySearch() {
    delete (searchResultsMenu);
    if (searchThread.joinable())
        searchThread.join();
}

void CitySearch::createSearchInput() {
    inputBox = Input(&searchContent, {
                                         .placeholder = "Location",
                                         .transform =
                                             [this](InputState state) {
                                                 state.element |=
                                                     color(textCol);
                                                 if (state.is_placeholder) {
                                                     state.element |= dim;
                                                 }

                                                 if (verticalSelector == 0) {
                                                     return hbox({
                                                         text("> "),
                                                         state.element,
                                                     });
                                                 } else {
                                                     state.element |= dim;
                                                 }

                                                 return state.element;
                                             },
                                     });
    inputBox |= CatchEvent([this](Event e) {
        if (e == Event::ArrowDown || e == Event::ArrowUp)
            return true;
        if (e == Event::Return) {
            verticalSelector = 1;
            return true;
        }
        if (e == Event::Escape) {
            toggleVisible();
            return true;
        }
        if (e.is_character() || e == Event::Backspace) {
            std::vector<std::string> cityNames;
            // searchResultsMenu->updateLabels(cityNames, true);
            if (isSearching == false) {
                startSearchThread();
            } else {
                updateSearchTime(searchTime);
            }
        }
        return false;
    });

    searchInputComponent = Renderer(inputBox, [this] {
        return hbox({
            filler(),
            inputBox->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 28),
            filler(),
        });
    });
}

void CitySearch::createSearchResults() {
    searchResultsMenu = new MenuComponent(
        {
            {"HELLO"},
            {"HELLO"},
        },
        std::nullopt, std::nullopt, [this](const EntryState& state) {
            std::string prefixSpace = " ";
            Element e = text(prefixSpace.append(state.label));
            if (state.active && verticalSelector == 1) {
                e = e | bold;
            }
            if (state.active && verticalSelector == 0) {
                e = e | color(textCol);
            }
            return e;
        });

    searchResultsMenuComponent = searchResultsMenu->getComponent();
    searchResultsMenuComponent |= CatchEvent([this](Event e) {
        if (e == Event::Escape) {
            verticalSelector = 0;
            return true;
        }
        if (e == Event::Return) {
            if (isSearching == true)
                return true;

            int selected = searchResultsMenu->getSelected();

            if (selected >= 0 && selected < foundCities.size()) {
                selectCallback(foundCities[selected]);
                toggleVisible();
            }

            return true;
        }
        return false;
    });

    searchResults = Renderer(searchResultsMenuComponent, [this] {
        auto sR = searchResultsMenuComponent->Render() | vscroll_indicator |
                  yframe | size(ftxui::HEIGHT, ftxui::EQUAL, 5) |
                  size(ftxui::WIDTH, ftxui::EQUAL, 30) | border;

        if (verticalSelector != 1)
            sR |= dim;

        return sR;
    });
}

void CitySearch::createCitySearch() {
    containerV = Container::Vertical({searchInputComponent, searchResults},
                                     &verticalSelector);

    citySearchComponent =
        Container::Horizontal({
            Renderer(containerV,
                     [this] {
                         return vbox({filler(),
                                      window(text(""), containerV->Render()) |
                                          clear_under,
                                      filler()}) |
                                center | bgcolor(Color::Black);
                     }),
        }) |
        center;

    citySearchComponent |= Maybe(&isVisible);
}

std::string formatCityName(std::string name) {
    if (name.length() > 25) {
        name = name.substr(0, 22) + "...";
    }
    return name;
}

void CitySearch::startSearchThread() {
    if (searchThread.joinable())
        searchThread.join();

    updateSearchTime(searchTime);
    isSearching = true;

    foundCities.clear();

    searchThread = std::thread([this] {
        auto loadingAnimationThread =
            std::thread([this] { // comment this to remove Loading... animation
                std::vector<std::string> animation = {".", "..", "..."};
                std::string loading = "Loading";
                int index = 0;
                while (isSearching == true) {
                    std::vector<std::string> sender = {
                        loading + animation[index],
                    };
                    index = (index + 1) % 3;
                    searchResultsMenu->updateLabels(sender, true);
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }
            });

        while (std::time(nullptr) < searchTime) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        auto returnedCitites = weatherFetcher::fetchCities(searchContent);

        std::vector<std::string> cityNames;

        for (auto city : returnedCitites) {
            cityNames.push_back(formatCityName(city.name));
        }
        isSearching = false;
        loadingAnimationThread.join();
        searchResultsMenu->updateLabels(cityNames, true);
        foundCities = returnedCitites;
    });
}
