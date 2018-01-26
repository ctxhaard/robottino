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
	int mm = -1;
	if (!_mm.empty()) mm = _mm.back();
	_newMeas = false;
	return mm;
}

void ProximitySensor::setMm(int mm) {
	if (_mm.size() >= FIFO_COUNT) {
		_mm.pop();
	}
	_mm.push(mm);
	_newMeas = true;
}

bool ProximitySensor::hasNewMeas() const
{
	return _newMeas;
}

bool ProximitySensor::isNotDecreasing() const
{
	if (_mm.size() <= 2) return false;
	// TODO: implement
	// archiviare gli ultimi 5 valori in una fifo
	// tornare true se il valore corrente e' >= alla media
	int avg = 0;
	std::queue<int> copy = _mm;
	while (!copy.empty()) {
		avg += copy.back();
		copy.pop();
	}	
	avg = avg / _mm.size();
	return (_mm.back() >= avg);
}

} // namespace ct
