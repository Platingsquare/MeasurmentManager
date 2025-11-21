#include "MeasurementStorage.h"
#include <iostream>

void MeasurementStorage::addMeasurement(const Measurement& m)
{
    m_data.push_back(m);
}

void MeasurementStorage::printAll() const
{
    if (m_data.empty()) {
        std::cout << "No measurements stored.\n";
        return;
    }
    for (std::size_t i = 0; i < m_data.size(); ++i) {
        const auto& m = m_data[i];
        std::cout << "[" << i << "] " << m.value << " (" << m.timestamp << ")\n";
    }
}
