#ifndef COLORMANAGER_H
#define COLORMANAGER_H
#include <colorType.hpp>
#include <ftxui/dom/elements.hpp>

class ColorManager {
  public:
    static ColorManager& getInstance();

    void
    initializeColors(std::vector<std::pair<ColorType, ftxui::Color>>& colors);

    void changeColors(std::vector<std::pair<ColorType, ftxui::Color>>& colors);

    ftxui::Color& get(ColorType colorType);

  private:
    std::unordered_map<ColorType, ftxui::Color> colorMap;

    ColorManager();
    ColorManager(const ColorManager&) = delete;
    ColorManager& operator=(const ColorManager&) = delete;
};

#endif
