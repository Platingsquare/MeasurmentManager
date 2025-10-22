C++ MeasurmentManager\Input.cpp
#include "MeasurmentManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

bool tryParseDouble(const std::string& token, double& out)
{
    if (token.empty()) return false;
    try
    {
        size_t idx = 0;
        out = std::stod(token, &idx);
        return (idx == token.size());
    }
    catch (...)
    {
        return false;
    }
}

void addMeasurements(std::vector<double>& data)
{
    std::cout << "Enter measurement values separated by spaces or commas.\n";
    std::cout << "Type \"done\" when finished.\n";

    while (true)
    {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) return;

        auto first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) continue;
        auto last = line.find_last_not_of(" \t\r\n");
        std::string trimmed = line.substr(first, last - first + 1);

        std::string lower = trimmed;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return std::tolower(c); });
        if (lower == "done") break;

        for (char& c : trimmed) if (c == ',') c = ' ';

        std::istringstream iss(trimmed);
        std::string token;
        bool foundAny = false;
        while (iss >> token)
        {
            double value;
            if (tryParseDouble(token, value))
            {
                data.push_back(value);
                foundAny = true;
            }
            else
            {
                std::cout << "  Warning: '" << token << "' is not a valid number and was skipped.\n";
            }
        }

        if (!foundAny) std::cout << "  No valid numbers in that line.\n";
    }

    std::cout << "Finished adding. Total measurements: " << data.size() << "\n";
}