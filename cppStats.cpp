C++ MeasurmentManager\Stats.cpp
#include "MeasurmentManager.h"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

Stats computeStats(const std::vector<double>& data)
{
    Stats s;
    s.count = data.size();
    if (s.count == 0) return s;

    s.sum = std::accumulate(data.begin(), data.end(), 0.0);
    s.mean = s.sum / static_cast<double>(s.count);

    auto mm = std::minmax_element(data.begin(), data.end());
    s.min = *mm.first;
    s.max = *mm.second;

    double accum = 0.0;
    for (double v : data)
    {
        double d = v - s.mean;
        accum += d * d;
    }

    s.variance = accum / static_cast<double>(s.count); // population variance
    s.stddev = std::sqrt(s.variance);
    return s;
}

void showStatistics(const std::vector<double>& data)
{
    if (data.empty())
    {
        std::cout << "No measurements available. Add values first.\n";
        return;
    }

    Stats s = computeStats(data);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Statistics:\n";
    std::cout << "  Count:                 " << s.count << "\n";
    std::cout << "  Sum:                   " << s.sum << "\n";
    std::cout << "  Mean:                  " << s.mean << "\n";
    std::cout << "  Min:                   " << s.min << "\n";
    std::cout << "  Max:                   " << s.max << "\n";
    std::cout << "  Variance (population): " << s.variance << "\n";
    std::cout << "  Standard deviation:    " << s.stddev << "\n";
}