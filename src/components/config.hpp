#ifndef CONFIG_H
#define CONFIG_H
#include "colorType.hpp"
#include "serializers.hpp"
#include <ftxui/component/component.hpp>

#define APP_TITLE "weather-cpp"
// TODO: look into paths, this is not good. Look at different OS paths
#define STYLE_CONFIG_LOCATION "/home/cobel/.config/weather-cpp/style.json"

static serializers::colorArray defaultColors = {
    {
        ColorType::TEXT,
        ftxui::Color::RGB(205, 214, 244),
    },
    {
        ColorType::HIGHLIGHT,
        ftxui::Color::RGB(250, 179, 135),
    },
};

void initializeConfig();

#endif
