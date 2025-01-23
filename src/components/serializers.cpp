#include "serializers.hpp"

using namespace std;
using namespace nlohmann;
using namespace ftxui;

ftxui::Color getColorFromHex(string hex) {
    string redHex = hex.substr(0, 2);
    string blueHex = hex.substr(2, 4);
    string greenHex = hex.substr(4, 6);
    uint8_t red = stoi(redHex, 0, 16);
    uint8_t blue = stoi(blueHex, 0, 16);
    uint8_t green = stoi(greenHex, 0, 16);

    return Color::RGB(red, green, blue);
}

serializers::colorMap
nlohmann::adl_serializer<serializers::colorMap>::from_json(
    const nlohmann::json& colList) {
    serializers::colorMap newColorMap;

    for (auto color : colList) {
        std::string name = color.at("name");
        std::string value = color.at("value");

        newColorMap[name] = getColorFromHex(value);
    }

    return newColorMap;
}
