C++ MeasurmentManager\Search.cpp
#include "MeasurmentManager.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

void searchValue(std::vector<double>& data)
{
    if (data.empty())
    {
        std::cout << "No measurements available to search.\n";
        return;
    }

    std::cout << "Enter value to search for: ";
    std::string line;
    if (!std::getline(std::cin, line)) return;

    double query;
    if (!tryParseDouble(line, query))
    {
        std::cout << "Invalid number.\n";
        return;
    }

    std::cout << "Enter epsilon tolerance (press Enter for default 1e-6): ";
    if (!std::getline(std::cin, line)) return;

    double eps = 1e-6;
    if (!line.empty())
    {
        if (!tryParseDouble(line, eps) || eps < 0.0)
        {
            std::cout << "Invalid epsilon. Using default 1e-6.\n";
            eps = 1e-6;
        }
    }

    std::vector<std::size_t> matches;
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        if (std::fabs(data[i] - query) <= eps) matches.push_back(i);
    }

    if (matches.empty())
    {
        std::cout << "No matches within epsilon = " << eps << ".\n";
        return;
    }

    std::cout << "Matches found:\n";
    for (std::size_t idx : matches) std::cout << "  Index " << idx << ": " << data[idx] << "\n";

    std::cout << "Enter indices to remove (space separated), or press Enter to keep all: ";
    if (!std::getline(std::cin, line)) return;

    auto firstNon = line.find_first_not_of(" \t\r\n");
    if (firstNon == std::string::npos) return;

    std::istringstream iss(line);
    std::string token;
    std::vector<int> toRemove;
    while (iss >> token)
    {
        try
        {
            size_t pos = 0;
            int idx = std::stoi(token, &pos);
            if (pos != token.size()) continue;
            if (idx < 0 || static_cast<std::size_t>(idx) >= data.size()) continue;
            if (std::find(matches.begin(), matches.end(), static_cast<std::size_t>(idx)) == matches.end()) continue;
            toRemove.push_back(idx);
        }
        catch (...) { /* ignore token */ }
    }

    if (toRemove.empty())
    {
        std::cout << "No valid indices to remove.\n";
        return;
    }

    std::sort(toRemove.begin(), toRemove.end());
    toRemove.erase(std::unique(toRemove.begin(), toRemove.end()), toRemove.end());
    std::sort(toRemove.rbegin(), toRemove.rend());

    for (int idx : toRemove)
    {
        data.erase(data.begin() + idx);
        std::cout << "  Removed index " << idx << ".\n";
    }

    std::cout << "Removal complete. New total measurements: " << data.size() << "\n";
}