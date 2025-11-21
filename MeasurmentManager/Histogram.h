#pragma once

#include <vector>

// Print a simple text-based bar graph to console.
// maxWidth controls the maximum number of characters used for the largest bar.
void printBarGraph(const std::vector<double>& values, int maxWidth = 50);