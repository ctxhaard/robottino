
#include "Pathfinder.h"
#include "MotorController.h"
#include "ProximitySensor.h"
#include <memory>

using namespace ct;

int main(int argc, const char *argv[]) {

	auto sensorRight = std::make_unique<ProximitySensor>("/dev/proximity0");
	auto sensorFront = std::make_unique<ProximitySensor>("/dev/proximity1");
	auto sensorLeft = std::make_unique<ProximitySensor>("/dev/proximity2");
	auto motorLeft = std::make_unique<MotorController>(0, 22, 24); 
	auto motorRight = std::make_unique<MotorController>(1, 23, 25);
	auto pathfinder = std::make_unique<Pathfinder>(std::move(motorLeft), std::move(motorRight), std::move(sensorFront));

	pathfinder->addLeftSensor(std::move(sensorLeft));
	pathfinder->addRightSensor(std::move(sensorRight));
	return pathfinder->run();
}
