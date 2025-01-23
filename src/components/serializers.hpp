#ifndef SERIALIZERS_H
#define SERIALIZERS_H
#include <ftxui/screen/color.hpp>
#include <nlohmann/json.hpp>

namespace serializers {

using colorMap = std::unordered_map<std::string, ftxui::Color>;

}; // namespace serializers

template <> struct nlohmann::adl_serializer<serializers::colorMap> {
    static serializers::colorMap from_json(const nlohmann::json& colList);
};

#endif
