#ifndef CONFIG_H
#define CONFIG_H
#include <ftxui/component/component.hpp>

#define APP_TITLE "weather-cpp"
// TODO: look into paths, this is not good. Look at different OS paths
#define STYLE_CONFIG_LOCATION "/home/cobel/.config/weather-cpp/style.json"

static std::unordered_map<std::string, ftxui::Color> defaultColors = {
    {
        "text",
        ftxui::Color::RGB(205, 214, 244),
    },
    {
        "highlight",
        ftxui::Color::RGB(250, 179, 135),
    },
};

void initializeConfig();

#endif
