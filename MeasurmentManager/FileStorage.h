#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include <string>
#include <vector>
#include "Measurement.h"

class FileStorage {
public:
    explicit FileStorage(const std::string& filename);

    // Load all measurements (timestamp + value). Skips header if present.
    std::vector<Measurement> load() const;

    // Append new measurements (uses measurement.timestamp already set).
    bool append(const std::vector<Measurement>& values) const;

    // Overwrite file with header and provided measurements (timestamp,value).
    bool saveAll(const std::vector<Measurement>& values) const;

    // Filename accessor
    const std::string& filename() const noexcept { return m_filename; }

private:
    std::string m_filename;
    static std::string makeTimestamp();
};

#endif // FILESTORAGE_H