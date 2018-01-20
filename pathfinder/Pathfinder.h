#pragma once
#include <memory>
#include <future>
#include <fstream>
#include <mutex>
#include "PFStatus.h"

#define FRONT_MM_MIN (250)
#define FRONT_MM_ALERT (300)
#define FRONT_MM_OK (300)
#define LATERAL_MM_MIN (250)

namespace ct {

class IMotorController;
class IProximitySensor;

class Pathfinder {

	friend class PFStatus;
public:
	Pathfinder(std::unique_ptr<IMotorController> &&leftMotor, std::unique_ptr<IMotorController> &&rightMotor, std::unique_ptr<IProximitySensor> &&frontSensor);

	void addLeftSensor(std::unique_ptr<IProximitySensor> &&s);
	void addRightSensor(std::unique_ptr<IProximitySensor> &&s);
	int run();

private:
	void setStatus(PFStatus *newStatus);

private:
	std::unique_ptr<IMotorController> _ml;
	std::unique_ptr<IMotorController> _mr;
	std::unique_ptr<IProximitySensor> _sf;
	std::unique_ptr<IProximitySensor> _sl;
	std::unique_ptr<IProximitySensor> _sr;

	std::unique_ptr<PFStatus>        _status;
	std::mutex                       _statusMutex;
	std::ofstream                    _display;
	std::mutex                       _displayMutex;
};
} /* namespace ct */

