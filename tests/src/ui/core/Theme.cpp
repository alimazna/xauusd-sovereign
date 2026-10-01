#include "Theme.h"

namespace xauusd::ui {

void apply_default_theme() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 4.0f;
    style.ChildRounding = 3.0f;
    style.FrameRounding = 3.0f;
    style.PopupRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;
    style.WindowPadding = ImVec2(10.0f, 10.0f);
    style.FramePadding = ImVec2(7.0f, 4.0f);
    style.ItemSpacing = ImVec2(7.0f, 5.0f);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = kDarkPalette[0];
    colors[ImGuiCol_ChildBg] = kDarkPalette[1];
    colors[ImGuiCol_PopupBg] = kDarkPalette[1];
    colors[ImGuiCol_FrameBg] = kDarkPalette[2];
    colors[ImGuiCol_FrameBgHovered] = kDarkPalette[3];
    colors[ImGuiCol_FrameBgActive] = kDarkPalette[4];
    colors[ImGuiCol_Border] = kDarkPalette[4];
    colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_Header] = kDarkPalette[3];
    colors[ImGuiCol_HeaderHovered] = kDarkPalette[4];
    colors[ImGuiCol_HeaderActive] = kDarkPalette[5];
}

} // namespace xauusd::ui
