#ifndef SERIALIZERS_H
#define SERIALIZERS_H
#include <ftxui/screen/color.hpp>
#include <nlohmann/json.hpp>

namespace serializers {

using colorArray = std::vector<std::pair<std::string, ftxui::Color>>;

}; // namespace serializers

template <> struct nlohmann::adl_serializer<serializers::colorArray> {
    static serializers::colorArray from_json(const nlohmann::json& colList);
};

#endif
