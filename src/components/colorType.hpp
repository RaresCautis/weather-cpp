#ifndef COLORS_H
#define COLORS_H

#include <optional>
#include <string>
#include <unordered_map>

enum class ColorType {
    TEXT,
    HIGHLIGHT,
};

// Function to map string to ColorType
inline std::optional<ColorType>
stringToColorType(const std::string& colorName) {
    static const std::unordered_map<std::string, ColorType> map = {
        {"text", ColorType::TEXT},
        {"highlight", ColorType::HIGHLIGHT},
    };

    auto it = map.find(colorName);
    if (it != map.end()) {
        return it->second;
    }
    return std::nullopt;
}

#endif
