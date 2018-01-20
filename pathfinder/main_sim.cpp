
#include "Pathfinder.h"
#include "MotorControllerSim.h"
#include "ProximitySensorSim.h"
#include <memory>

using namespace ct;

Pathfinder *g_pathfinder;

int main(int argc, const char *argv[]) {
	auto sensorRight = std::make_unique<ProximitySensorSim>();
	auto sensorFront = std::make_unique<ProximitySensorSim>();
	auto sensorLeft = std::make_unique<ProximitySensorSim>();
	auto motorLeft = std::make_unique<MotorControllerSim>(); 
	auto motorRight = std::make_unique<MotorControllerSim>();
	auto pathfinder = std::make_unique<Pathfinder>(std::move(motorLeft), std::move(motorRight), std::move(sensorFront));

	g_pathfinder = pathfinder.get();

	pathfinder->addLeftSensor(std::move(sensorLeft));
	pathfinder->addRightSensor(std::move(sensorRight));
	return pathfinder->run();
}
