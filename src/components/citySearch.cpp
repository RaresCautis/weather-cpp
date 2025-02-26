#include "colorManager.hpp"
#include <citySearch.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/screen/color.hpp>
#include <menu.hpp>
#include <optional>

using namespace ftxui;

CitySearch::CitySearch(citySearch::CityData* newReturnCity,
                       std::function<void()> newChangeFocus) {
    returnCity = newReturnCity;
    changeFocus = newChangeFocus;

    ColorManager& colMan = ColorManager::getInstance();
    textCol = colMan.get(ColorType::TEXT);

    createSearchInput();
    createSearchResults();
    createCitySearch();
}

CitySearch::~CitySearch() { delete (searchResultsMenu); }

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

void CitySearch::createSearchInput() {
    inputBox = Input({
                   .placeholder = "City Name",
                   .transform =
                       [this](InputState state) {
                           state.element |= color(textCol);
                           if (state.is_placeholder) {
                               state.element |= dim;
                           }

                           if (verticalSelector == 0) {
                               return hbox({
                                   text("> "),
                                   state.element,
                               });
                           }

                           return state.element;
                       },
               }) |
               CatchEvent([this](Event e) {
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

    // citySearchComponent = Container::Horizontal({
    //     Button({"xd"}),
    //     Button({"xd"}),
    //     Button({"xd"}),
    //     Button({"xd"}),
    // });

    citySearchComponent |= Maybe(&isVisible);
}
