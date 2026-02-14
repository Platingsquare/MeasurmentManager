#pragma once

#include "Sensor.h"
#include <random>

class TemperatureSensor : public Sensor {
public:
    explicit TemperatureSensor(const SensorConfig& cfg);

    // Implement read() (non-const)
    double read() override;

private:
    std::mt19937 rng_;
    std::uniform_real_distribution<double> dist_;
};