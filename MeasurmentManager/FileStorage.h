#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include <string>
#include <vector>

class FileStorage {
public:
    explicit FileStorage(const std::string& filename);

    // Load all numeric values from the file (ignores timestamps).
    // Returns empty vector if file missing or unreadable.
    std::vector<double> load() const;

    // Append new values to the file, each with a timestamp.
    // Returns true on success.
    bool append(const std::vector<double>& values) const;

    // Replace file contents with provided values (each line gets a timestamp).
    bool saveAll(const std::vector<double>& values) const;

    // Filename accessor
    const std::string& filename() const noexcept { return m_filename; }

private:
    std::string m_filename;

    // Produce a human-readable timestamp (YYYY-MM-DD HH:MM:SS)
    static std::string makeTimestamp();
};

#endif // FILESTORAGE_H