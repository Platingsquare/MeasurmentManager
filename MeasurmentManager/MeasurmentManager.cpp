// MeasurmentManager.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <limits>
#include <cmath>
#include <iomanip>
#include <cctype>
#include "FileStorage.h"
#include "utilities.h"

class Timestamper
{
    std::time_t t = utils::now();
    std::string ts = utils::timestamp_to_string(t); // timestamp as string to be put wherever needed
};


class Stats
{
public:
    std::size_t count = 0;
    double sum = 0.0;
    double mean = std::numeric_limits<double>::quiet_NaN();
    double min = std::numeric_limits<double>::quiet_NaN();
    double max = std::numeric_limits<double>::quiet_NaN();
    double variance = std::numeric_limits<double>::quiet_NaN(); // population variance
    double stddev = std::numeric_limits<double>::quiet_NaN();
};

// Try to parse a double from a token. Returns true on success.
// Accepts integers and decimals. Rejects tokens with trailing non-numeric chars.
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

// Compute statistics for the provided dataset (population variance).
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

// Print computed statistics to console. Handles empty dataset.
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

// Add measurements. Accepts multiple numbers per line separated by spaces or commas.
// Type "done" (case-insensitive) on its own line to finish adding.
void addMeasurements(std::vector<double>& data)
{
    std::cout << "Enter measurement values separated by spaces or commas.\n";
    std::cout << "Type \"done\" when finished.\n";

    while (true)
    {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line))
            return; 

        // Trim leading/trailing whitespace
        auto first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) continue;
        auto last = line.find_last_not_of(" \t\r\n");
        std::string trimmed = line.substr(first, last - first + 1);

        // Check for done (case-insensitive)
        std::string lower = trimmed;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return std::tolower(c); });
        if (lower == "done") break;

        // Replace commas with spaces so tokenization is uniform
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

        if (!foundAny)
            std::cout << "  No valid numbers in that line.\n";
    }

    std::cout << "Finished adding. Total measurements: " << data.size() << "\n";
}

// Search for a value within an epsilon tolerance. Default epsilon used if user presses Enter.
void searchValue(const std::vector<double>& data)
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

    std::cout << "Enter tolerance (press Enter to continue): ";
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

    bool any = false;
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        if (std::fabs(data[i] - query) <= eps)
        {
            if (!any)
            {
                std::cout << "Matches found:\n";
                any = true;
            }
            std::cout << "  Index " << i << ": " << data[i] << "\n";
        }
    }

    if (!any)
        std::cout << "No matches within epsilon = " << eps << ".\n";
}

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

void printMenu()
{
    std::cout << "\nMeasurement Manager - Menu\n";
    std::cout << "1. Add new measurements\n";
    std::cout << "2. Show statistics\n";
    std::cout << "3. Search for a value\n";
    std::cout << "4. Sort the list of measurements (ascending/descending)\n";
    std::cout << "5. Exit program\n";
    std::cout << "Choose an option (1-5): ";
}

int main()

{
    std::cout << "Welcome to the Measurement Manager!\n";
    FileStorage storage("measurements.csv");
    std::vector<double> measurements = storage.load(); // load any previously saved values

    while (true)
    {
        printMenu();

        std::string choiceLine;
        if (!std::getline(std::cin, choiceLine)) break; // EOF

        // Allow whitespace; try parse first integer
        int choice = 0;
        try
        {
            size_t pos = 0;
            choice = std::stoi(choiceLine, &pos);
            (void)pos; // accept leading integer
        }
        catch (...)
        {
            std::cout << "Invalid choice. Enter a number between 1 and 5.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            addMeasurements(measurements);
            storage.saveAll(measurements); // overwrites file with current list
            break;
        case 2:
            showStatistics(measurements);
            break;
        case 3:
            searchValue(measurements);
            break;
        case 4:
            sortMeasurements(measurements);
            break;
        case 5:
            std::cout << "Exiting program.\n";
            return 0;
        default:
            std::cout << "Invalid choice. Enter a number between 1 and 5.\n";
            break;
        }
    }

    return 0;

}