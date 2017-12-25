#pragma once
#include <memory>
#include <future>
#include <fstream>
#include "PFStatus.h"

namespace ct {

class MotorController;
class ProximitySensor;

class Pathfinder {

	friend class PFStatus;
public:
	Pathfinder(std::unique_ptr<MotorController> &&leftMotor, std::unique_ptr<MotorController> &&rightMotor, std::unique_ptr<ProximitySensor> &&frontSensor);

	void addLeftSensor(std::unique_ptr<ProximitySensor> &&s);
	void addRightSensor(std::unique_ptr<ProximitySensor> &&s);
	int run();

private:
	std::unique_ptr<MotorController> _ml;
	std::unique_ptr<MotorController> _mr;
	std::unique_ptr<ProximitySensor> _sf;
	std::unique_ptr<ProximitySensor> _sl;
	std::unique_ptr<ProximitySensor> _sr;

	std::unique_ptr<PFStatus>        _status;
	std::ofstream                    _display;
};
} /* namespace ct */

