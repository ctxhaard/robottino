#include "ProximitySensor.h"
#include <fstream>
#include <iostream>

#define FIFO_COUNT (10)

namespace ct
{
ProximitySensor::ProximitySensor(std::string devPath)
	: _devPath(devPath)
//	, _mm{-1}
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
				int lastVal = -1;
				while (!_stopRequested && is.is_open()) {
					//std::cout << "proximity read ";
					int val;
					is >> val;
					if (val != lastVal) {
						lastVal = val;
						setMm(val);
						callback(val);
					}
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
	int mm = -1;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_mm.empty()) mm = _mm.back();
		_newMeas = false;
	}
	return mm;
}

void ProximitySensor::setMm(int mm) {
	std::lock_guard<std::mutex> lock(_mutex);
	if (_mm.size() >= FIFO_COUNT) {
		_mm.pop_front();
	}
	_mm.push_back(mm);
	_newMeas = true;
}

bool ProximitySensor::hasNewMeas() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _newMeas;
}

bool ProximitySensor::isNotDecreasing() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_mm.size() <= 2) return false;
	float avg = 0;
	int size = _mm.size();
	int val = _mm.back();
	
	for (auto v : _mm) {
		avg += v;
	}

	avg = avg / size;
	return (val >= avg);
}

} // namespace ct
