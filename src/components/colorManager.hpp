#ifndef COLORMANAGER_H
#define COLORMANAGER_H
#include <ftxui/dom/elements.hpp>

class ColorManager {
  public:
    static ColorManager& getInstance();

    void
    initializeColors(std::vector<std::pair<std::string, ftxui::Color>>& colors);

    void
    changeColors(std::vector<std::pair<std::string, ftxui::Color>>& colors);

    ftxui::Color& get(std::string colorName);

  private:
    std::unordered_map<std::string, ftxui::Color> colorMap;

    ColorManager();
    ColorManager(const ColorManager&) = delete;
    ColorManager& operator=(const ColorManager&) = delete;
};

#endif
