#include <chrono>
#include "utilities.h"
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>

namespace utils {
	std::time_t now() {
		auto now = std::chrono::system_clock::now();
		auto result = std::chrono::system_clock::to_time_t(now);
		return result;
	}

	std::string timestamp_to_string(std::time_t timestamp) {
		std::tm tm;
		gmtime_s(&tm, &timestamp);  // I used gmtime for UTC time
		std::stringstream ss;
		ss << std::put_time(&tm, "%F %T");
		return ss.str();
	}
}