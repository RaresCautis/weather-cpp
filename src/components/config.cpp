#include "config.hpp"
#include "colorManager.hpp"
#include "serializers.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace {
using namespace nlohmann;

// TODO: Need to handle errors here
serializers::colorArray readColorJson() {
    std::ifstream styleFile(STYLE_CONFIG_LOCATION);
    auto jsonData = json::parse(styleFile);

    return jsonData.template get<serializers::colorArray>();
}
}; // namespace

void initializeConfig() {
    ColorManager& colMan = ColorManager::getInstance();
    auto colors = readColorJson();
    colMan.initializeColors(defaultColors);
    colMan.changeColors(colors);
}
