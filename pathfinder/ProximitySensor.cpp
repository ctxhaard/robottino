#include "ProximitySensor.h"
#include <fstream>

namespace ct
{
ProximitySensor::ProximitySensor(std::string devPath)
	: _devPath(devPath)
{
	
}

std::future<int> ProximitySensor::acquire(std::function<void(int)> callback)
{
	auto result = std::async(std::launch::async,[this,callback]() -> int {
				std::ifstream is(_devPath);
				while (is.is_open()) {
					int mm;
					is >> mm;
					callback(mm);
				}
				return 0;
			});	
	return result;
}
} // namespace ct
