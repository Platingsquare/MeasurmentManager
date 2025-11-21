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
#include "Sensor.h"
#include "MeasurementStorage.h"
#include "Measurement.h"
#include "Timestamp.h"

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

void printMenu()
{
    std::cout << "\nMeasurement Manager - Menu\n";
    std::cout << "1. Add new measurements\n";
    std::cout << "2. Show statistics\n";
    std::cout << "3. Search for a value\n";
    std::cout << "4. Sort the list of measurements (ascending/descending)\n";
    std::cout << "5. Simulate sensors (read & store)\n";
    std::cout << "6. Exit program\n";
    std::cout << "Choose an option (1-6): ";
}

// Simulate two sensors, store their readings into MeasurementStorage and persist via FileStorage
void simulateSensors(MeasurementStorage& storage, FileStorage& file)
{
    // Example default configs; in practice load from CSV via SensorConfig::loadFromCsv
    SensorConfig cfg1{ "TempSensor 1", "°C", -10.0, 40.0, 30.0 };
    SensorConfig cfg2{ "Humidity 1", "%", 0.0, 100.0, 80.0 };

    Sensor tempSensor(cfg1);
    Sensor humSensor(cfg2);

    double tval = tempSensor.read();
    double hval = humSensor.read();

    Measurement m1{ currentTimestamp(), tval };
    Measurement m2{ currentTimestamp(), hval };

    storage.addMeasurement(m1);
    storage.addMeasurement(m2);

    // Persist appended measurements
    std::vector<Measurement> batch{ m1, m2 };
    if (!file.append(batch)) {
        std::cout << "Warning: failed to append sensor readings to file.\n";
    }

    std::cout << "Sensor readings stored:\n";
    std::cout << "  " << cfg1.name << ": " << m1.value << " " << cfg1.unit << " (" << m1.timestamp << ")\n";
    std::cout << "  " << cfg2.name << ": " << m2.value << " " << cfg2.unit << " (" << m2.timestamp << ")\n";
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

    std::cout << "Enter tolerance (press Enter for default 1e-6): ";
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

int main()
{
    std::cout << "Welcome to the Measurement Manager!\n";
    FileStorage storage("measurements.csv");

    // load existing measurements into MeasurementStorage
    MeasurementStorage mstorage;
    auto loaded = storage.load();
    for (const auto& m : loaded) mstorage.addMeasurement(m);

    while (true)
    {
        printMenu();

        std::string choiceLine;
        if (!std::getline(std::cin, choiceLine)) break; // EOF

        int choice = 0;
        try
        {
            size_t pos = 0;
            choice = std::stoi(choiceLine, &pos);
            (void)pos; // accept leading integer
        }
        catch (...)
        {
            std::cout << "Invalid choice. Enter a number between 1 and 6.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            // add measurements interactively
        {
            std::cout << "Enter a value (or 'done' to stop):\n";
            while (true) {
                std::cout << "> ";
                std::string line;
                if (!std::getline(std::cin, line)) break;
                if (line.empty()) continue;
                std::string lower = line;
                std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){ return std::tolower(c); });
                if (lower == "done") break;
                double v;
                if (!tryParseDouble(line, v)) {
                    std::cout << "Invalid number.\n";
                    continue;
                }
                Measurement m{ currentTimestamp(), v };
                mstorage.addMeasurement(m);
                // persist single measurement
                if (!storage.append(std::vector<Measurement>{m}))
                    std::cout << "Warning: failed to save measurement.\n";
                std::cout << "Added: " << m.value << " at " << m.timestamp << "\n";
            }
            break;
        }
        case 2:
            // show statistics computed over values in mstorage
        {
            std::vector<double> vals;
            for (const auto& mm : mstorage.getAll()) vals.push_back(mm.value);
            showStatistics(vals);
            break;
        }
        case 3:
        {
            std::vector<double> vals;
            for (const auto& mm : mstorage.getAll()) vals.push_back(mm.value);
            searchValue(vals);
            break;
        }
        case 4:
        {
            // sort storage by value and print first 10
            auto all = mstorage.getAll();
            std::vector<Measurement> mod = all; // make a modifiable copy
            std::sort(mod.begin(), mod.end(), [](const Measurement&a,const Measurement&b){ return a.value < b.value; });
            std::cout << "First up to 10 values after sort:\n";
            for (size_t i=0;i<std::min<size_t>(10, mod.size());++i)
                std::cout << "  ["<<i<<"] = "<<mod[i].value<<" ("<<mod[i].timestamp<<")\n";
            break;
        }
        case 5:
            simulateSensors(mstorage, storage);
            break;
        case 6:
            std::cout << "Exiting program.\n";
            return 0;
        default:
            std::cout << "Invalid choice. Enter a number between 1 and 6.\n";
            break;
        }
    }

    return 0;
}