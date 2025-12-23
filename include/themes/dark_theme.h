#pragma once
#include <imgui.h>

// 🌙 Rams Dark Theme v3 (Compatible with all ImGui versions)
inline void SetDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Backgrounds
    colors[ImGuiCol_WindowBg]       = ImVec4(0.08f, 0.09f, 0.11f, 0.92f);
    colors[ImGuiCol_ChildBg]        = ImVec4(0.10f, 0.12f, 0.15f, 0.75f);
    colors[ImGuiCol_PopupBg]        = ImVec4(0.13f, 0.14f, 0.18f, 0.95f);

    // Texts
    colors[ImGuiCol_Text]           = ImVec4(0.93f, 0.94f, 0.96f, 1.0f);
    colors[ImGuiCol_TextDisabled]   = ImVec4(0.45f, 0.48f, 0.52f, 1.0f);

    // Controls
    colors[ImGuiCol_Button]         = ImVec4(0.20f, 0.40f, 0.75f, 1.0f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.25f, 0.50f, 0.90f, 1.0f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.15f, 0.35f, 0.60f, 1.0f);

    colors[ImGuiCol_Header]         = ImVec4(0.25f, 0.50f, 0.85f, 1.0f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.16f, 0.18f, 0.22f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.25f, 0.30f, 1.0f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.25f, 0.35f, 0.50f, 1.0f);

    colors[ImGuiCol_CheckMark]      = ImVec4(0.36f, 0.62f, 0.98f, 1.0f);
    colors[ImGuiCol_SliderGrab]     = ImVec4(0.36f, 0.62f, 0.98f, 1.0f);
    colors[ImGuiCol_SliderGrabActive]=ImVec4(0.26f, 0.52f, 0.90f, 1.0f);

    // Style tweaks for “soft shadow” feeling
    style.FrameRounding   = 7.0f;
    style.WindowRounding  = 12.0f;
    style.GrabRounding    = 6.0f;
    style.ScrollbarRounding = 10.0f;
    style.WindowBorderSize = 1.0f;

    // Simulated translucent shadow via alpha
    colors[ImGuiCol_WindowBg].w = 0.90f;
}
