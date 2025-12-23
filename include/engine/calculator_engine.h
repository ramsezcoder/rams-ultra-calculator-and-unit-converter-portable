#pragma once
#include <string>

class CalculatorEngine {
public:
    CalculatorEngine();
    void Render();

private:
    double Evaluate(const std::string& expr);
    double result;
};
