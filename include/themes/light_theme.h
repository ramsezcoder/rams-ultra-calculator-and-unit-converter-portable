#pragma once
#include <imgui.h>

// ☀️ Rams Light Theme v3 (Compatible + readable)
inline void SetLightTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg]       = ImVec4(0.97f, 0.98f, 0.99f, 0.92f);
    colors[ImGuiCol_ChildBg]        = ImVec4(0.98f, 0.99f, 1.0f, 0.88f);
    colors[ImGuiCol_PopupBg]        = ImVec4(1.00f, 1.00f, 1.00f, 0.96f);

    colors[ImGuiCol_Text]           = ImVec4(0.12f, 0.14f, 0.16f, 1.0f);
    colors[ImGuiCol_TextDisabled]   = ImVec4(0.45f, 0.48f, 0.52f, 1.0f);

    colors[ImGuiCol_Button]         = ImVec4(0.38f, 0.64f, 0.95f, 1.0f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.48f, 0.74f, 1.00f, 1.0f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.28f, 0.54f, 0.90f, 1.0f);

    colors[ImGuiCol_Header]         = ImVec4(0.65f, 0.80f, 1.00f, 1.0f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.92f, 0.94f, 0.97f, 1.0f);

    colors[ImGuiCol_CheckMark]      = ImVec4(0.25f, 0.45f, 0.90f, 1.0f);
    colors[ImGuiCol_SliderGrab]     = ImVec4(0.36f, 0.62f, 0.98f, 1.0f);
    colors[ImGuiCol_SliderGrabActive]=ImVec4(0.26f, 0.52f, 0.90f, 1.0f);

    style.FrameRounding   = 7.0f;
    style.WindowRounding  = 12.0f;
    style.GrabRounding    = 6.0f;
    style.ScrollbarRounding = 10.0f;
    style.WindowBorderSize = 1.0f;

    // Subtle alpha blur feel
    colors[ImGuiCol_WindowBg].w = 0.94f;
}
