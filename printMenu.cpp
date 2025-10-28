C++ MeasurmentManager\MeasurmentManager.cpp
#include "MeasurmentManager.h"
#include "FileStorage.h"
#include <iostream>
#include <string>

// Simple menu printing
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
            // persist entire list after additions
            if (!storage.saveAll(measurements))
                std::cout << "Warning: failed to save measurements to file.\n";
            break;
        case 2:
            showStatistics(measurements);
            break;
        case 3:
            searchValue(measurements);
            break;
        case 4:
            sortMeasurements(measurements);
            // optional save after sorting:
            if (!storage.saveAll(measurements))
                std::cout << "Warning: failed to save measurements to file.\n";
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