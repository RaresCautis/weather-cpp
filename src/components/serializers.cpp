#include <colorType.hpp>
#include <serializers.hpp>

using namespace std;
using namespace nlohmann;
using namespace ftxui;

namespace {

ftxui::Color getColorFromHex(string hex) {
    string redHex = hex.substr(0, 2);
    string greenHex = hex.substr(2, 2);
    string blueHex = hex.substr(4, 2);
    int red = stoi(redHex, 0, 16);
    int green = stoi(greenHex, 0, 16);
    int blue = stoi(blueHex, 0, 16);

    return Color::RGB(red, green, blue);
}

}; // namespace

serializers::colorArray
nlohmann::adl_serializer<serializers::colorArray>::from_json(
    const nlohmann::json& colList) {
    serializers::colorArray newColorMap;

    for (auto color : colList) {
        std::string name = color.at("name");
        auto type = stringToColorType(name);

        if (!type)
            continue;

        std::string value = color.at("value");

        newColorMap.push_back({
            *type,
            getColorFromHex(value),
        });
    }

    return newColorMap;
}
