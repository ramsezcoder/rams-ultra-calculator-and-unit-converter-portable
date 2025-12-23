#include "ui.h"
#include <imgui.h>
#include <algorithm>

// ============================================================
// 🧱 KONSTRUKTOR UTAMA
// ============================================================
UI::UI() : mode(Mode::CALCULATOR), themeIndex(0), transition(0.0f) {
    // Simpan style awal untuk transisi tema
    SetDarkTheme();
    darkStyle = ImGui::GetStyle();
    SetLightTheme();
    lightStyle = ImGui::GetStyle();
    SetDarkTheme();

    // High-DPI scaling
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.5f;          // perbesar teks (150%)
    ImGui::GetStyle().ScaleAllSizes(1.5f); // ukuran elemen ikut besar
}

// ============================================================
// 🎨 TRANSISI HALUS ANTARA DARK DAN LIGHT
// ============================================================
void UI::BlendTheme(float t) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* darkColors = darkStyle.Colors;
    ImVec4* lightColors = lightStyle.Colors;
    ImVec4* colors = style.Colors;

    for (int i = 0; i < ImGuiCol_COUNT; i++) {
        colors[i].x = darkColors[i].x + (lightColors[i].x - darkColors[i].x) * t;
        colors[i].y = darkColors[i].y + (lightColors[i].y - darkColors[i].y) * t;
        colors[i].z = darkColors[i].z + (lightColors[i].z - darkColors[i].z) * t;
        colors[i].w = darkColors[i].w + (lightColors[i].w - darkColors[i].w) * t;
    }
}

// ============================================================
// 🌙 DARK THEME STYLE (MODERN GELAP)
// ============================================================
void UI::SetDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    style.FrameRounding = 6.0f;
    style.WindowRounding = 10.0f;
    style.ScrollbarSize = 22.0f;
    style.FramePadding = ImVec2(12, 10);
    style.ItemSpacing = ImVec2(10, 10);
    style.WindowPadding = ImVec2(12, 12);

    colors[ImGuiCol_WindowBg]       = ImVec4(0.09f, 0.10f, 0.12f, 1.0f);
    colors[ImGuiCol_ChildBg]        = ImVec4(0.11f, 0.12f, 0.14f, 1.0f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.20f, 0.23f, 0.28f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.28f, 0.31f, 0.36f, 1.0f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.35f, 0.39f, 0.45f, 1.0f);
    colors[ImGuiCol_Button]         = ImVec4(0.23f, 0.26f, 0.32f, 1.0f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.30f, 0.33f, 0.40f, 1.0f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.35f, 0.38f, 0.45f, 1.0f);
    colors[ImGuiCol_Header]         = ImVec4(0.25f, 0.28f, 0.35f, 1.0f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.35f, 0.38f, 0.45f, 1.0f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.40f, 0.44f, 0.52f, 1.0f);
    colors[ImGuiCol_Text]           = ImVec4(0.95f, 0.96f, 0.98f, 1.0f);
}

// ============================================================
// ☀️ LIGHT THEME STYLE (BERSIH HD)
// ============================================================
void UI::SetLightTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    style.FrameRounding = 6.0f;
    style.WindowRounding = 10.0f;
    style.ScrollbarSize = 22.0f;
    style.FramePadding = ImVec2(12, 10);
    style.ItemSpacing = ImVec2(10, 10);
    style.WindowPadding = ImVec2(12, 12);

    colors[ImGuiCol_WindowBg]       = ImVec4(0.94f, 0.95f, 0.97f, 1.0f);
    colors[ImGuiCol_ChildBg]        = ImVec4(0.97f, 0.98f, 0.99f, 1.0f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.89f, 0.90f, 0.93f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.83f, 0.85f, 0.88f, 1.0f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.78f, 0.80f, 0.84f, 1.0f);
    colors[ImGuiCol_Button]         = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    colors[ImGuiCol_Header]         = ImVec4(0.35f, 0.38f, 0.42f, 1.0f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.45f, 0.48f, 0.52f, 1.0f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.50f, 0.54f, 0.58f, 1.0f);
    colors[ImGuiCol_Text]           = ImVec4(0.06f, 0.06f, 0.08f, 1.0f);
}

// ============================================================
// 🧮 RENDER UTAMA UI
// ============================================================
void UI::Render() {
    ImGui::SetNextWindowSize(ImVec2(850, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Rams Ultra Calculator & Unit Converter", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    // ========================
    // 🌗 Theme toggle control
    // ========================
    ImGui::Text("Theme:");
    ImGui::SameLine();
    int newTheme = themeIndex;
    ImGui::RadioButton("Dark", &newTheme, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Light", &newTheme, 1);

    // Saat user ubah tema → trigger animasi fade
    if (newTheme != themeIndex) {
        themeIndex = newTheme;
        transition = 1.0f;
    }

    // Jalankan animasi blending warna tema
    if (transition > 0.0f) {
        float t = (themeIndex == 0) ? (1.0f - transition) : transition;
        BlendTheme(t);
        transition -= ImGui::GetIO().DeltaTime * 3.0f;
        if (transition < 0.0f) {
            transition = 0.0f;
            if (themeIndex == 0)
                SetDarkTheme();
            else
                SetLightTheme();
        }
    }

    ImGui::Separator();

    // ========================
    // 📁 TAB MODE
    // ========================
    if (ImGui::BeginTabBar("Modes")) {
        if (ImGui::BeginTabItem("Calculator")) {
            calc.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Converter")) {
            conv.Render();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::Separator();

    // ========================
    // 🕒 HISTORY SECTION
    // ========================
    hist.Render();

    ImGui::End();
}
