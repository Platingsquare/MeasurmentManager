#pragma once

#include <string>
#include <random>
#include <vector>

// Configuration for a sensor
struct SensorConfig {
    std::string name;    // sensor name, e.g. "TempSensor 1"
    std::string unit;    // unit, e.g. "°C"
    double minValue = 0.0; // simulation interval min
    double maxValue = 1.0; // simulation interval max
    double threshold = 0.0; // threshold value

    // Load configs from simple CSV file with columns:
    // name,unit,min,max,threshold
    // Lines starting with '#' or a header containing "name" are skipped.
    static std::vector<SensorConfig> loadFromCsv(const std::string& filename);
};

// Sensor that produces simulated readings according to its SensorConfig.
class Sensor {
public:
    explicit Sensor(const SensorConfig& cfg);

    // Return sensor configuration
    const SensorConfig& config() const noexcept { return cfg_; }

    // Generate a simulated reading (random double within [minValue,maxValue])
    double read();

private:
    SensorConfig cfg_;
    std::mt19937 rng_;
    std::uniform_real_distribution<double> dist_;
};
