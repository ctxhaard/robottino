#pragma once
#include <memory>
#include <future>
#include <fstream>
#include <mutex>
#include "PFStatus.h"

#define FRONT_MM_MIN (150)
#define FRONT_MM_ALERT (200)
#define FRONT_MM_OK (250)
#define LATERAL_MM_MIN (150)

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
	std::mutex                       _statusMutex;
	std::ofstream                    _display;
	std::mutex                       _displayMutex;
};
} /* namespace ct */

