#include <string>
#include <ctime>

namespace utils {
	namespace timezones {
		const std::time_t ONE_HOUR = 3600;
	}
	std::time_t now();
	std::time_t locale(std::time_t,std::time_t);
	std::string timestamp_to_string(std::time_t timestamp);
}
