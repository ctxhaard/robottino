
#include "Pathfinder.h"
#include "MotorController.h"
#include "ProximitySensor.h"
#include <memory>
#include <signal.h>
#include <iostream>

using namespace ct;

Pathfinder *g_pathfinder;

void handle_signal(int signal)
{
	g_pathfinder->stop();
	std::cout << "RECEIVED SIGNAL: " << signal << std::endl;
}

int main(int argc, const char *argv[]) {
	auto sensorRight = std::make_unique<ProximitySensor>("/dev/proximity2");
	auto sensorFront = std::make_unique<ProximitySensor>("/dev/proximity1");
	auto sensorLeft = std::make_unique<ProximitySensor>("/dev/proximity0");
	auto motorLeft = std::make_unique<MotorController>(0, 22, 24); 
	auto motorRight = std::make_unique<MotorController>(1, 23, 25);
	auto pathfinder = std::make_unique<Pathfinder>(std::move(motorLeft), std::move(motorRight), std::move(sensorFront));

	g_pathfinder = pathfinder.get();
	signal(SIGINT, handle_signal);

	pathfinder->addLeftSensor(std::move(sensorLeft));
	pathfinder->addRightSensor(std::move(sensorRight));
	return pathfinder->run();
}
