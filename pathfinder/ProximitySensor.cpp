#include "ProximitySensor.h"
#include <fstream>
#include <iostream>

namespace ct
{
ProximitySensor::ProximitySensor(std::string devPath)
	: _devPath(devPath), _mm{-1}
{
	
}

std::future<int> ProximitySensor::acquire(std::function<void(int)> callback)
{
	auto result = std::async(std::launch::async,[this,callback]() -> int {
				std::cout << "proximity async lambda" << std::endl;
				std::ifstream is(_devPath);
				while (is.is_open()) {
					//std::cout << "proximity read ";
					int mm;
					is >> mm;
					_mm = mm;
					//std::cout << mm << std::endl;
					callback(mm);
				}
				return 0;
			});	
	return result;
}

int ProximitySensor::getMm() const {
	return _mm;
}
} // namespace ct
