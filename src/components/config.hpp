#ifndef CONFIG_H
#define CONFIG_H
#include <ftxui/component/component.hpp>

#define APP_TITLE "weather-cpp"

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
