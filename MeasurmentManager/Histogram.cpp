#include "Histogram.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>

void printBarGraph(const std::vector<double>& values, int maxWidth)
{
    if (values.empty()) return;

    double minVal = *std::min_element(values.begin(), values.end());
    double maxVal = *std::max_element(values.begin(), values.end());
    double range = maxVal - minVal;

    std::cout << "\nBar Graph:\n";
    for (size_t i = 0; i < values.size(); ++i) {
        double v = values[i];
        std::cout << "Value #" << (i + 1) << " (" << std::fixed << std::setprecision(6) << v << "): ";
        int barLength = 0;
        if (range > 0.0) {
            double normalized = (v - minVal) / range; // 0..1
            barLength = static_cast<int>(std::round(normalized * maxWidth));
        }
        else {
            // all values equal; draw a mid-sized bar
            barLength = maxWidth / 2;
        }
        for (int j = 0; j < barLength; ++j) std::cout << '#';
        std::cout << '\n';
    }
}
