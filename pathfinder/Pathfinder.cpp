/*
 * Pathfinder.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: ctomasin
 */

#include "Pathfinder.h"
#include "MotorController.h"
#include "ProximitySensor.h"
#include "PFStatus.h"
#include <iomanip>
//#include <chrono>
//#include <thread>

namespace ct {

Pathfinder::Pathfinder(std::unique_ptr<MotorController> &&leftMotor, std::unique_ptr<MotorController> &&rightMotor, std::unique_ptr<ProximitySensor> &&frontSensor)
	: _ml{ std::move(leftMotor) }
	, _mr{ std::move(rightMotor) }
	, _sf{ std::move(frontSensor) }
	, _display{ "/dev/lcd0" }
{
	// NOTE: devo usare una move() perché una r-value reference è un lvalue (così ho trovato scritto...)
}

int Pathfinder::run() {
	_status = std::make_unique<PFStatusRolling>(*this); 
	auto fs = _sf->acquire([this](int mm) {
			{
				std::lock_guard<std::mutex> lock(_displayMutex);
				_display.seekp(16 + 5);
				_display << std::left << std::setw(5) << std::setfill('.') << (mm / 10);
			}
			_status->onFrontSensor(*this, mm);
			});

	auto ls = _sl->acquire([this](int mm) {
			{
				std::lock_guard<std::mutex> lock(_displayMutex);
				_display.seekp(16);
				_display << std::left << std::setw(5) << std::setfill('.') << (mm / 10);
			}
			_status->onLeftSensor(*this, mm);
			});

	auto rs = _sr->acquire([this](int mm) {
			{
				std::lock_guard<std::mutex> lock(_displayMutex);
				_display.seekp(16 + 10);
				_display << std::right << std::setw(5) << std::setfill('.') << (mm / 10);
			}
			_status->onRightSensor(*this, mm);
			});

	fs.get();
	ls.get();
	rs.get();

	_ml->roll();
	_mr->roll();
	return 0;
}

void Pathfinder::addLeftSensor( std::unique_ptr<ProximitySensor> &&s)
{
	_sl = std::move(s);
}

void Pathfinder::addRightSensor( std::unique_ptr<ProximitySensor> &&s)
{
	_sr = std::move(s);
}

void Pathfinder::setStatus(PFStatus *newStatus)
{
	std::lock_guard<std::mutex> statusLock(_statusMutex);
	_status->end();
	//std::this_thread::sleep_for( std::chrono::milliseconds( 1500 ));
	_status.reset(newStatus);
	_status->begin();
}


} /* namespace ct */
