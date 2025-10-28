C++ MeasurmentManager\sortMeasurements.cpp
#include "MeasurmentManager.h"
#include <iostream>
#include <algorithm>
#include <cctype>

// Sort the stored measurements ascending or descending.
void sortMeasurements(std::vector<double>& data)
{
    if (data.empty())
    {
        std::cout << "No measurements available to sort.\n";
        return;
    }

    std::cout << "Choose sort order: (A)scending or (D)escending [A/D]: ";
    std::string line;
    if (!std::getline(std::cin, line)) return;
    char c = line.empty() ? 'A' : std::toupper(static_cast<unsigned char>(line[0]));

    if (c == 'D')
    {
        std::sort(data.begin(), data.end(), std::greater<double>());
        std::cout << "Sorted descending.\n";
    }
    else
    {
        std::sort(data.begin(), data.end());
        std::cout << "Sorted ascending.\n";
    }

    // Show up to first 10 values to confirm
    std::cout << "First up to 10 values after sort:\n";
    for (std::size_t i = 0; i < std::min<std::size_t>(10, data.size()); ++i)
        std::cout << "  [" << i << "] = " << data[i] << "\n";
}