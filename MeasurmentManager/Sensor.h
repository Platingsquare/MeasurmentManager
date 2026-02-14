#pragma once

#include <string>
#include <vector>

// Configuration for a sensor
struct SensorConfig {
    std::string name;
    std::string unit;
    double minValue = 0.0;
    double maxValue = 1.0;
    double threshold = 0.0;

    static std::vector<SensorConfig> loadFromCsv(const std::string& filename);
};

// Abstract base class for sensors
class Sensor {
public:
    explicit Sensor(const SensorConfig& cfg) : cfg_(cfg) {}
    virtual ~Sensor() = default;

    // Non-const pure virtual read() so implementations can mutate RNG state
    virtual double read() = 0;

    // Accessors - return const refs to avoid unnecessary copies
    const std::string& name() const { return cfg_.name; }
    const std::string& unit() const { return cfg_.unit; }
    const SensorConfig& config() const { return cfg_; }

    // Prevent copying/slicing (derived sensors often hold RNG state)
    Sensor(const Sensor&) = delete;
    Sensor& operator=(const Sensor&) = delete;

protected:
    SensorConfig cfg_;
};