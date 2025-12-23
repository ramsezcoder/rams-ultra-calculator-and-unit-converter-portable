#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

struct ConverterCategory {
    std::string name;
    std::unordered_map<std::string, double> units;
    std::string baseUnit;
};

class ConverterEngine {
public:
    ConverterEngine();
    ~ConverterEngine();

    void Render();
    bool IsCurrencyCategory() const; // ✅ ditambahkan untuk deteksi kategori aktif

private:
    std::vector<ConverterCategory> categories;
    int currentCategory;
    int fromIndex, toIndex;
    double value, result;

    // --- Currency Data ---
    std::unordered_map<std::string, double> liveRates;
    std::chrono::steady_clock::time_point lastUpdate;
    bool ratesLoaded;

    void AddCategory(const ConverterCategory& c);
    void RenderCategory(const ConverterCategory& c);
    void UpdateCurrencyRates();
};
