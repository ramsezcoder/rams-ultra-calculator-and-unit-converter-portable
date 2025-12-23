#pragma once
#include "engine/calculator_engine.h"
#include "engine/converter_engine.h"
#include "engine/history_manager.h"
#include <imgui.h>

class UI {
public:
    UI();
    void Render();

private:
    enum Mode { CALCULATOR, CONVERTER };
    Mode mode;

    int themeIndex;     // 0 = Dark, 1 = Light
    float transition;   // animasi blending

    ImGuiStyle darkStyle;
    ImGuiStyle lightStyle;

    CalculatorEngine calc;
    ConverterEngine conv;
    HistoryManager hist;

    void SetDarkTheme();
    void SetLightTheme();
    void BlendTheme(float t);
};
