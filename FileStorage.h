C++ MeasurmentManager\FileStorage.h
#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include <string>
#include <vector>

class FileStorage {
public:
    explicit FileStorage(const std::string& filename);

    // Load numeric values (ignores timestamps). Returns empty if missing/unreadable.
    std::vector<double> load() const;

    // Append records (timestamp,value) lines.
    bool append(const std::vector<double>& values) const;

    // Overwrite file with header and provided values (timestamp,value).
    bool saveAll(const std::vector<double>& values) const;

    const std::string& filename() const noexcept { return m_filename; }

private:
    std::string m_filename;
    static std::string makeTimestamp();
};

#endif // FILESTORAGE_H