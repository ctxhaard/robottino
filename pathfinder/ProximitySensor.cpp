#include "ProximitySensor.h"
#include <fstream>
#include <iostream>

namespace ct
{
ProximitySensor::ProximitySensor(std::string devPath)
	: _devPath(devPath), _mm{-1}, _newMeas{false}
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
					setMm(mm);
					//std::cout << mm << std::endl;
					callback(mm);
				}
				return 0;
			});	
	return result;
}

int ProximitySensor::getMm() {
	_newMeas = false;
	return _mm;
}

void ProximitySensor::setMm(int mm) {
	_mm = mm;
	_newMeas = true;
}

bool ProximitySensor::hasNewMeas() const
{
	return _newMeas;
}
} // namespace ct
