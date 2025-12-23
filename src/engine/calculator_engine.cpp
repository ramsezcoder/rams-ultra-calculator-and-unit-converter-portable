#include "engine/calculator_engine.h"
#include <imgui.h>
#include <sstream>
#include <string>
#include <cstring>

CalculatorEngine::CalculatorEngine() : result(0.0) {}

double CalculatorEngine::Evaluate(const std::string& e) {
    std::stringstream ss(e);
    double val, res = 0; char op = '+';
    while (ss >> val) {
        if (op == '+') res += val;
        else if (op == '-') res -= val;
        else if (op == '*') res *= val;
        else if (op == '/') res /= val;
        ss >> op;
    }
    return res;
}

void CalculatorEngine::Render() {
    static std::string expr = "";
    ImGui::Text("Scientific Calculator");
    ImGui::InputText("Expression", expr.data(), expr.capacity() + 1);

    const char* buttons[] = {
        "7","8","9","/",
        "4","5","6","*",
        "1","2","3","-",
        "0",".","=","+"
    };
    for (int i = 0; i < 16; i++) {
        if (i % 4 != 0) ImGui::SameLine();
        if (ImGui::Button(buttons[i], ImVec2(50, 50))) {
            if (strcmp(buttons[i], "=") == 0) result = Evaluate(expr);
            else expr += buttons[i];
        }
    }
    if (ImGui::Button("Clear", ImVec2(210, 30))) expr.clear();
    ImGui::Text("Result: %.6f", result);
}
