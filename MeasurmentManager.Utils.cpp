C++ MeasurmentManager\Utils.cpp
#include "MeasurmentManager.h"
#include <string>

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