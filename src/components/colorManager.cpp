#include "colorManager.hpp"

using namespace ftxui;
using namespace std;

ColorManager::ColorManager() {};

ColorManager& ColorManager::getInstance() {
    static ColorManager instance;
    return instance;
}

void ColorManager::initializeColors(vector<pair<ColorType, Color>>& colors) {
    for (auto col : colors) {
        auto color = col.first;
        auto value = col.second;

        colorMap[color] = value;
    }
}

void ColorManager::changeColors(vector<pair<ColorType, Color>>& colors) {
    for (auto col : colors) {
        auto color = col.first;
        auto value = col.second;

        if (colorMap.find(color) != colorMap.end())
            colorMap[color] = value;
    }
}

// TODO: Rethink this and error handle
Color& ColorManager::get(ColorType colorType) { return colorMap[colorType]; }
