#pragma once

#include <array>

#include <imgui.h>

namespace xauusd::ui {

inline const std::array<ImVec4, 6> kDarkPalette = {{
    ImVec4(0.055f, 0.062f, 0.075f, 1.0f),
    ImVec4(0.090f, 0.100f, 0.120f, 1.0f),
    ImVec4(0.130f, 0.145f, 0.175f, 1.0f),
    ImVec4(0.210f, 0.230f, 0.270f, 1.0f),
    ImVec4(0.320f, 0.345f, 0.400f, 1.0f),
    ImVec4(0.550f, 0.575f, 0.640f, 1.0f),
}};

void apply_default_theme();

} // namespace xauusd::ui
