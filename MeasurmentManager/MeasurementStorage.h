#pragma once

#include "Measurement.h"
#include <vector>

class MeasurementStorage {
public:
    MeasurementStorage() = default;

    // Add a measurement
    void addMeasurement(const Measurement& m);

    // Print all stored measurements (index, value and timestamp)
    void printAll() const;

    // Access stored measurements
    const std::vector<Measurement>& getAll() const noexcept { return m_data; }

    // Utility
    std::size_t size() const noexcept { return m_data.size(); }
    bool empty() const noexcept { return m_data.empty(); }

private:
    std::vector<Measurement> m_data;
};
