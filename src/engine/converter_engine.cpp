#include "engine/converter_engine.h"
#include <imgui.h>
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <future>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <curl/curl.h>

// ======================================
// ⚙️ LOGGING SYSTEM
// ======================================
static void LogToFile(const std::string& msg) {
    std::ofstream log("converter_log.txt", std::ios::app);
    if (log.is_open()) {
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        log << "[" << std::ctime(&t) << "] " << msg << "\n";
        log.close();
    }
    std::cout << msg << std::endl;
}

// ======================================
// CURL WRITE CALLBACK
// ======================================
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append((char*)contents, totalSize);
    return totalSize;
}

// ======================================
// ASYNC HANDLER
// ======================================
static std::future<void> asyncUpdater;
static bool updatingNow = false;

// ======================================
// 🌍 CURRENCY NAME MAP (Sorted Alphabetically)
// ======================================
static std::unordered_map<std::string, std::string> CurrencyFullNames = {
    {"AED","UAE Dirham"}, {"AUD","Australian Dollar"}, {"BDT","Bangladeshi Taka"},
    {"BHD","Bahraini Dinar"}, {"BRL","Brazilian Real"}, {"CAD","Canadian Dollar"},
    {"CHF","Swiss Franc"}, {"CNY","Chinese Yuan"}, {"CZK","Czech Koruna"},
    {"DKK","Danish Krone"}, {"EGP","Egyptian Pound"}, {"EUR","Euro"},
    {"GBP","British Pound"}, {"HKD","Hong Kong Dollar"}, {"IDR","Indonesian Rupiah"},
    {"ILS","Israeli Shekel"}, {"INR","Indian Rupee"}, {"JPY","Japanese Yen"},
    {"KRW","South Korean Won"}, {"KWD","Kuwaiti Dinar"}, {"LKR","Sri Lankan Rupee"},
    {"MAD","Moroccan Dirham"}, {"MYR","Malaysian Ringgit"}, {"MXN","Mexican Peso"},
    {"NGN","Nigerian Naira"}, {"NOK","Norwegian Krone"}, {"NZD","New Zealand Dollar"},
    {"PHP","Philippine Peso"}, {"PKR","Pakistani Rupee"}, {"PLN","Polish Zloty"},
    {"QAR","Qatari Riyal"}, {"RON","Romanian Leu"}, {"RUB","Russian Ruble"},
    {"SAR","Saudi Riyal"}, {"SEK","Swedish Krona"}, {"SGD","Singapore Dollar"},
    {"THB","Thai Baht"}, {"TRY","Turkish Lira"}, {"TWD","Taiwan Dollar"},
    {"USD","United States Dollar"}, {"VND","Vietnamese Dong"}, {"ZAR","South African Rand"}
};

// ======================================
// CONSTRUCTOR / DESTRUCTOR
// ======================================
ConverterEngine::ConverterEngine()
    : currentCategory(0), fromIndex(0), toIndex(1), value(0.0), result(0.0), ratesLoaded(false)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    LogToFile("ConverterEngine initialized.");

    AddCategory({"Length", {
        {"Meter", 1.0}, {"Kilometer", 1000.0}, {"Centimeter", 0.01}, {"Millimeter", 0.001},
        {"Mile", 1609.34}, {"Yard", 0.9144}, {"Foot", 0.3048}, {"Inch", 0.0254}
    }, "Meter"});

    AddCategory({"Weight and Mass", {
        {"Kilogram", 1.0}, {"Gram", 0.001}, {"Milligram", 1e-6}, {"Pound", 0.453592}, {"Ounce", 0.0283495}
    }, "Kilogram"});

    AddCategory({"Volume", {
        {"Cubic Meter", 1.0}, {"Liter", 0.001}, {"Milliliter", 1e-6},
        {"Gallon (US)", 0.00378541}, {"Quart (US)", 0.000946353}
    }, "Cubic Meter"});

    AddCategory({"Speed", {
        {"m/s", 1.0}, {"km/h", 1000.0/3600.0}, {"mi/h", 1609.34/3600.0},
        {"ft/s", 0.3048}, {"knot", 1852.0/3600.0}
    }, "m/s"});

    AddCategory({"Temperature", {}, "Kelvin"});
    AddCategory({"Currency", { {"USD", 1.0} }, "USD"});
}

ConverterEngine::~ConverterEngine() {
    curl_global_cleanup();
    LogToFile("ConverterEngine destroyed.");
}

// ======================================
// CATEGORY HANDLING
// ======================================
void ConverterEngine::AddCategory(const ConverterCategory& c) {
    categories.push_back(c);
}

bool ConverterEngine::IsCurrencyCategory() const {
    if (currentCategory >= 0 && currentCategory < (int)categories.size())
        return categories[currentCategory].name == "Currency";
    return false;
}

void ConverterEngine::Render() {
    ImGui::Text("Universal Converter");
    ImGui::Separator();

    std::vector<std::string> names;
    for (auto& c : categories) names.push_back(c.name);

    ImGui::Combo("Category", &currentCategory, [](void* data, int idx, const char** out_text) {
        auto* v = (std::vector<std::string>*)data;
        *out_text = (*v)[idx].c_str();
        return true;
    }, (void*)&names, names.size());

    if (currentCategory >= 0 && currentCategory < (int)categories.size())
        RenderCategory(categories[currentCategory]);
}

// ======================================
// CATEGORY RENDERER
// ======================================
void ConverterEngine::RenderCategory(const ConverterCategory& c) {
    ImGui::InputDouble("Value", &value);

    std::vector<std::string> unitNames;
    for (auto& kv : c.units)
        unitNames.push_back(kv.first);

    // ---------- Temperature ----------
    if (c.name == "Temperature") {
        const char* tempUnits[] = {"Kelvin","Celsius","Fahrenheit","Rankine","Reaumur"};
        static int fromT = 0, toT = 1;
        ImGui::Combo("From", &fromT, tempUnits, IM_ARRAYSIZE(tempUnits));
        ImGui::Combo("To", &toT, tempUnits, IM_ARRAYSIZE(tempUnits));

        double v=value, res=0.0, k=0.0;
        if(fromT==0) k=v;
        else if(fromT==1) k=v+273.15;
        else if(fromT==2) k=(v+459.67)*5.0/9.0;
        else if(fromT==3) k=v*5.0/9.0;
        else if(fromT==4) k=v*1.25+273.15;

        if(toT==0) res=k;
        else if(toT==1) res=k-273.15;
        else if(toT==2) res=k*9.0/5.0-459.67;
        else if(toT==3) res=k*9.0/5.0;
        else if(toT==4) res=(k-273.15)*0.8;

        result=res;
        ImGui::Text("Converted: %.4f %s", result, tempUnits[toT]);
        return;
    }

    // ---------- Currency ----------
    if (c.name == "Currency") {
        if (!ratesLoaded && !updatingNow) UpdateCurrencyRates();
        else if (ratesLoaded && std::chrono::duration_cast<std::chrono::minutes>(
                 std::chrono::steady_clock::now() - lastUpdate).count() > 10 && !updatingNow)
            UpdateCurrencyRates();

        if (updatingNow)
            ImGui::TextDisabled("⏳ Updating rates...");
        else if (!ratesLoaded)
            ImGui::TextDisabled("⚠ No rates loaded.");

        unitNames.clear();
        for (auto& kv : liveRates) unitNames.push_back(kv.first);
        if (unitNames.empty()) unitNames.push_back("USD");

        std::sort(unitNames.begin(), unitNames.end());
    }

    if (unitNames.empty()) { ImGui::TextDisabled("⚠ No units available."); return; }

    if (fromIndex >= (int)unitNames.size()) fromIndex = 0;
    if (toIndex >= (int)unitNames.size()) toIndex = 1 % unitNames.size();

    // Combo label generator
    auto comboLabel = [&](int& idx, const char* label){
        ImGui::Combo(label, &idx, [](void* data, int idx, const char** out_text){
            auto* v = (std::vector<std::string>*)data;
            static std::string display;
            std::string code = (*v)[idx];

            // hanya currency yang diberi nama panjang
            if (auto* engine = reinterpret_cast<ConverterEngine*>(ImGui::GetStateStorage()))
                display = code;
            else display = code;

            *out_text = display.c_str();
            return true;
        }, (void*)&unitNames, unitNames.size());
    };

    // Render combos
    comboLabel(fromIndex, "From");
    comboLabel(toIndex, "To");

    // Conversion logic
    if (c.name == "Currency" && !liveRates.empty()) {
        auto from = unitNames[fromIndex];
        auto to = unitNames[toIndex];
        if (liveRates.count(from) && liveRates.count(to)) {
            double base = value / liveRates[from];
            result = base * liveRates[to];
            std::string label = CurrencyFullNames.count(to) ? CurrencyFullNames[to] : to;
            ImGui::Text("Live Converted: %.4f %s", result, label.c_str());
        } else ImGui::TextDisabled("⚠ Invalid currency data.");
        ImGui::SameLine();
        ImGui::TextDisabled("(Auto-updated every 10 min)");
    } else if (c.name != "Currency") {
        const auto& fromUnit = unitNames[fromIndex];
        const auto& toUnit = unitNames[toIndex];
        if (c.units.count(fromUnit) && c.units.count(toUnit)) {
            double baseVal = value * c.units.at(fromUnit);
            result = baseVal / c.units.at(toUnit);
            ImGui::Text("Converted: %.8f %s", result, toUnit.c_str());
        } else ImGui::TextDisabled("⚠ Invalid unit conversion.");
    }
}

// ======================================
// 🌐 CURRENCY API UPDATE (ASYNC)
// ======================================
void ConverterEngine::UpdateCurrencyRates() {
    if (updatingNow) return;
    updatingNow = true;

    asyncUpdater = std::async(std::launch::async, [this]() {
        LogToFile("[Currency] Background update started...");

        CURL* curl = curl_easy_init();
        if (!curl) { LogToFile("[Currency] curl_easy_init() failed."); updatingNow = false; return; }

        std::string buffer;
        curl_easy_setopt(curl, CURLOPT_URL, "https://open.er-api.com/v6/latest/USD");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) { LogToFile("[Currency] curl failed: " + std::string(curl_easy_strerror(res))); updatingNow = false; return; }
        if (buffer.empty()) { LogToFile("[Currency] Empty API response."); updatingNow = false; return; }

        size_t pos = buffer.find("\"rates\":");
        if (pos == std::string::npos) { LogToFile("[Currency] No rates field found."); updatingNow = false; return; }

        size_t start = buffer.find("{", pos);
        size_t end = buffer.find("}", start);
        if (start == std::string::npos || end == std::string::npos) { LogToFile("[Currency] JSON parse error."); updatingNow = false; return; }

        std::string ratesStr = buffer.substr(start + 1, end - start - 1);
        std::istringstream ss(ratesStr);
        std::string pair;
        std::unordered_map<std::string, double> tempRates;

        while (std::getline(ss, pair, ',')) {
            size_t colon = pair.find(':');
            if (colon == std::string::npos) continue;
            std::string key = pair.substr(1, pair.find('"', 1) - 1);
            double val = atof(pair.substr(colon + 1).c_str());
            if (!key.empty() && val > 0) tempRates[key] = val;
        }

        if (!tempRates.empty()) {
            liveRates = std::move(tempRates);
            ratesLoaded = true;
            lastUpdate = std::chrono::steady_clock::now();
            LogToFile("[Currency] Rates updated successfully. Total: " + std::to_string(liveRates.size()));
        } else LogToFile("[Currency] No valid rates parsed.");

        updatingNow = false;
    });
}
