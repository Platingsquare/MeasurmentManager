#include "FileStorage.h"
#include "Timestamp.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

FileStorage::FileStorage(const std::string& filename)
    : m_filename(filename)
{
}

std::string FileStorage::makeTimestamp()
{
    return currentTimestamp();
}

std::vector<Measurement> FileStorage::load() const
{
    std::vector<Measurement> result;
    std::ifstream in(m_filename);
    if (!in.is_open()) return result;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        // Expect "timestamp,value"
        auto pos = line.find_last_of(',');
        if (pos == std::string::npos) continue;
        std::string ts = line.substr(0, pos);
        std::string valueToken = line.substr(pos + 1);

        // trim ts
        auto firstTs = ts.find_first_not_of(" \t\r\n");
        if (firstTs == std::string::npos) continue;
        auto lastTs = ts.find_last_not_of(" \t\r\n");
        ts = ts.substr(firstTs, lastTs - firstTs + 1);

        // skip header lines
        std::string lowerTs = ts;
        for (char &c : lowerTs) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (lowerTs.find("timestamp") != std::string::npos) continue;

        // trim valueToken
        auto first = valueToken.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) continue;
        auto last = valueToken.find_last_not_of(" \t\r\n");
        valueToken = valueToken.substr(first, last - first + 1);

        try {
            size_t idx = 0;
            double v = std::stod(valueToken, &idx);
            if (idx == valueToken.size()) {
                result.push_back(Measurement{ ts, v });
            }
        } catch (...) {
            continue;
        }
    }
    return result;
}

bool FileStorage::append(const std::vector<Measurement>& values) const
{
    std::ofstream out(m_filename, std::ios::app);
    if (!out.is_open()) return false;

    for (const auto& m : values) {
        std::string ts = m.timestamp.empty() ? makeTimestamp() : m.timestamp;
        out << ts << ',' << m.value << '\n';
    }
    return out.good();
}

bool FileStorage::saveAll(const std::vector<Measurement>& values) const
{
    std::ofstream out(m_filename, std::ios::trunc);
    if (!out.is_open()) return false;

    out << "timestamp,value\n";
    for (const auto& m : values) {
        std::string ts = m.timestamp.empty() ? makeTimestamp() : m.timestamp;
        out << ts << ',' << m.value << '\n';
    }
    return out.good();
};