#include "colorManager.hpp"

using namespace ftxui;
using namespace std;

// TODO: Maybe implement this as a map from enum to color

ColorManager::ColorManager() {};

ColorManager& ColorManager::getInstance() {
    static ColorManager instance;
    return instance;
}

void ColorManager::initializeColors(vector<pair<string, Color>>& colors) {
    for (auto col : colors) {
        auto name = col.first;
        auto value = col.second;

        colorMap[name] = value;
    }
}

void ColorManager::changeColors(vector<pair<string, Color>>& colors) {
    for (auto col : colors) {
        auto name = col.first;
        auto value = col.second;

        if (colorMap.find(name) != colorMap.end())
            colorMap[name] = value;
    }
}

// TODO: Rethink this and error handle
Color& ColorManager::get(string colorName) { return colorMap[colorName]; }
