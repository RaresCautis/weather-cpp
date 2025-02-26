#ifndef SERIALIZERS_H
#define SERIALIZERS_H
#include <colorType.hpp>
#include <ftxui/screen/color.hpp>
#include <nlohmann/json.hpp>
#include <weatherFetcher.hpp>

namespace serializers {

using colorArray = std::vector<std::pair<ColorType, ftxui::Color>>;

}; // namespace serializers

template <> struct nlohmann::adl_serializer<serializers::colorArray> {
    static serializers::colorArray from_json(const nlohmann::json& colList);
};

template <> struct nlohmann::adl_serializer<weatherFetcher::weatherData> {
    static weatherFetcher::weatherData from_json(const nlohmann::json&);
};

template <> struct nlohmann::adl_serializer<std::vector<citySearch::CityData>> {
    static std::vector<citySearch::CityData> from_json(const nlohmann::json&);
};

#endif
