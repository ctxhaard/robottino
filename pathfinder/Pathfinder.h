#pragma once
#include <memory>
#include <future>
#include <fstream>
#include <mutex>
#include "PFStatus.h"

#define FRONT_MM_MIN    (300)
#define FRONT_MM_ALERT  (350)
#define FRONT_MM_OK     (400)
#define LATERAL_MM_MIN  (150)
#define LATERAL_MM_GOOD (300)

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
	void stop();

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

	bool _stopRequested;
};
} /* namespace ct */

