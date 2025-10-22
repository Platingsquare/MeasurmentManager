C++ MeasurmentManager\MeasurmentManager.h
#ifndef MEASURMENT_MANAGER_H
#define MEASURMENT_MANAGER_H

#include <vector>
#include <string>
#include <cstddef>

struct Stats
{
    std::size_t count = 0;
    double sum = 0.0;
    double mean = 0.0;
    double min = 0.0;
    double max = 0.0;
    double variance = 0.0; // population variance by default
    double stddev = 0.0;
};

// Input
bool tryParseDouble(const std::string& token, double& out);
void addMeasurements(std::vector<double>& data);

// Stats
Stats computeStats(const std::vector<double>& data);
void showStatistics(const std::vector<double>& data);

// Search (note: non-const vector to allow removals)
void searchValue(std::vector<double>& data);

// Sort
void sortMeasurements(std::vector<double>& data);

// UI
void printMenu();

#endif // MEASURMENT_MANAGER_H