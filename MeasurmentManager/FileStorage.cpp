#include "FileStorage.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

FileStorage::FileStorage(const std::string& filename)
    : m_filename(filename)
{
}

std::string FileStorage::makeTimestamp()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    std::tm tm;
#if defined(_MSC_VER)
    localtime_s(&tm, &t);
#else
    tm = *std::localtime(&t);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::vector<double> FileStorage::load() const
{
    std::vector<double> result;
    std::ifstream in(m_filename);
    if (!in.is_open()) return result;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        // Expect either: timestamp, value  OR  value
        // Find last comma; token after comma is treated as value.
        std::string valueToken = line;
        auto pos = line.find_last_of(',');
        if (pos != std::string::npos) {
            valueToken = line.substr(pos + 1);
        }
        // trim whitespace
        auto first = valueToken.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) continue;
        auto last = valueToken.find_last_not_of(" \t\r\n");
        valueToken = valueToken.substr(first, last - first + 1);

        try {
            size_t idx = 0;
            double v = std::stod(valueToken, &idx);
            if (idx == valueToken.size()) result.push_back(v);
        } catch (...) {
            // skip non-numeric lines
            continue;
        }
    }
    return result;
}

bool FileStorage::append(const std::vector<double>& values) const
{
    std::ofstream out(m_filename, std::ios::app);
    if (!out.is_open()) return false;

    // If file is empty, optionally write a header
    // (simple check: if file position == 0)
    // Note: using separate ifstream to check length is more robust, but keep basic.
    for (double v : values) {
        out << makeTimestamp() << "," << v << "\n";
    }
    return out.good();
}

bool FileStorage::saveAll(const std::vector<double>& values) const
{
    std::ofstream out(m_filename, std::ios::trunc);
    if (!out.is_open()) return false;

    out << "timestamp,value\n";
    for (double v : values) {
        out << makeTimestamp() << "," << v << "\n";
    }
    return out.good();
}