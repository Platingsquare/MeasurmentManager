/*
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

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
        if (!std::getline(std::cin, line)) // EOF or error -> return
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
*/