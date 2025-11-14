#include <string>
#include <ctime>

namespace utils {
	std::time_t now();
	std::string timestamp_to_string(std::time_t timestamp);
}
