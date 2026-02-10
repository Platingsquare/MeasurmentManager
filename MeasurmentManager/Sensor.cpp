#include "Sensor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <random>
// Helper trim function
static inline std::string trim(const std::string& s) {
    auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return std::string();
    auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

std::vector<SensorConfig> SensorConfig::loadFromCsv(const std::string& filename)
{
    std::vector<SensorConfig> out;
    std::ifstream in(filename);
    if (!in.is_open()) return out;

    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (line.size() > 0 && line[0] == '#') continue;

        // split by comma
        std::vector<std::string> parts;
        std::string token;
        std::istringstream ss(line);
        while (std::getline(ss, token, ',')) {
            parts.push_back(trim(token));
        }
        if (parts.empty()) continue;

        // skip header line if it looks like one
        std::string low = parts[0];
        std::transform(low.begin(), low.end(), low.begin(), [](unsigned char c){ return std::tolower(c); });
        if (low.find("name") != std::string::npos) continue;

        SensorConfig cfg;
        cfg.name = parts.size() > 0 ? parts[0] : std::string();
        cfg.unit = parts.size() > 1 ? parts[1] : std::string();

        auto parseDouble = [&](size_t i, double def) {
            if (i < parts.size() && !parts[i].empty()) {
                try { return std::stod(parts[i]); } catch (...) { return def; }
            }
            return def;
        };

        cfg.minValue = parseDouble(2, 0.0);
        cfg.maxValue = parseDouble(3, 1.0);
        cfg.threshold = parseDouble(4, 0.0);

        // ensure min<=max
        if (cfg.minValue > cfg.maxValue) std::swap(cfg.minValue, cfg.maxValue);

        out.push_back(cfg);
    }

    return out;
}