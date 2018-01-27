#include "ProximitySensorSim.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

namespace ct
{
ProximitySensorSim::ProximitySensorSim()
	: _mm{-1}, _newMeas{false}
{
	
}

std::future<int> ProximitySensorSim::acquire(std::function<void(int)> callback)
{
	auto result = std::async(std::launch::async,[this,callback]() -> int {
				std::cout << "proximity async lambda" << std::endl;
				while (true) {
					std::this_thread::sleep_for( std::chrono::milliseconds( 100 ));
					//std::cout << "proximity read ";
					int mm = rand() % 1000;
					setMm(mm);
					//std::cout << mm << std::endl;
					callback(mm);
				}
				return 0;
			});	
	return result;
}

void ProximitySensorSim::stop()
{
	std::cout << "PROXIMITY SENSOR GOT A STOP REQUEST\n";
}

int ProximitySensorSim::getMm() {
	_newMeas = false;
	return _mm;
}

void ProximitySensorSim::setMm(int mm) {
	_mm = mm;
	_newMeas = true;
}

bool ProximitySensorSim::hasNewMeas() const
{
	return _newMeas;
}

bool ProximitySensorSim::isNotDecreasing() const
{
	// TODO: implement
	return false;
}
} // namespace ct
