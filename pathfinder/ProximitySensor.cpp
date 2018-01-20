#include "ProximitySensor.h"
#include <fstream>
#include <iostream>

namespace ct
{
ProximitySensor::ProximitySensor(std::string devPath)
	: _devPath(devPath)
	, _mm{-1}
	, _newMeas{false}
	, _stopRequested{false}
{
	
}

std::future<int> ProximitySensor::acquire(std::function<void(int)> callback)
{
	_stopRequested = false;
	auto result = std::async(std::launch::async,[this,callback]() -> int {
				std::cout << "proximity async lambda" << std::endl;
				std::ifstream is(_devPath);
				while (!_stopRequested && is.is_open()) {
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

void ProximitySensor::stop()
{
	_stopRequested = true;
	std::cout << "PROXIMITY SENSOR GOT A STOP REQUEST\n";
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
