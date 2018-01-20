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
#include <iostream>
#include <chrono>
#include <thread>

namespace ct {

Pathfinder::Pathfinder(std::unique_ptr<IMotorController> &&leftMotor, std::unique_ptr<IMotorController> &&rightMotor, std::unique_ptr<IProximitySensor> &&frontSensor)
	: _ml{ std::move(leftMotor) }
	, _mr{ std::move(rightMotor) }
	, _sf{ std::move(frontSensor) }
	, _display{ "/dev/lcd0" }
{
	// NOTE: devo usare una move() perché una r-value reference è un lvalue (così ho trovato scritto...)
}

int Pathfinder::run() {
	std::mutex loopMutex;
	loopMutex.lock();
	_status = std::make_unique<PFStatusRolling>(*this); 
	auto fs = _sf->acquire([this, &loopMutex](int mm) {
				loopMutex.unlock();
				std::lock_guard<std::mutex> lock(_displayMutex);
				_display.seekp(16 + 5);
				_display << std::left << std::setw(5) << std::setfill('.') << (mm / 10);
			});

	auto ls = _sl->acquire([this, &loopMutex](int mm) {
				loopMutex.unlock();
				std::lock_guard<std::mutex> lock(_displayMutex);
				_display.seekp(16);
				_display << std::left << std::setw(5) << std::setfill('.') << (mm / 10);
			});

	auto rs = _sr->acquire([this, &loopMutex](int mm) {
				loopMutex.unlock();
				std::lock_guard<std::mutex> lock(_displayMutex);
				_display.seekp(16 + 10);
				_display << std::right << std::setw(5) << std::setfill('.') << (mm / 10);
			});

	while(true) {
		{
			std::lock_guard<std::mutex> lock(loopMutex);
		}
		if (_sf->hasNewMeas()) {
			_status->onFrontSensor(*this, _sf->getMm());
		}
		if (_sl->hasNewMeas()) {
			_status->onLeftSensor(*this, _sl->getMm());
		}
		if (_sr->hasNewMeas()) {
			_status->onRightSensor(*this, _sr->getMm());
		}
	};

	fs.get();
	ls.get();
	rs.get();

	_ml->roll();
	_mr->roll();
	return 0;
}

void Pathfinder::addLeftSensor( std::unique_ptr<IProximitySensor> &&s)
{
	_sl = std::move(s);
}

void Pathfinder::addRightSensor( std::unique_ptr<IProximitySensor> &&s)
{
	_sr = std::move(s);
}

void Pathfinder::setStatus(PFStatus *newStatus)
{
	std::lock_guard<std::mutex> statusLock(_statusMutex);
	_status->end();
	std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ));
	_status.reset(newStatus);
	_status->begin();
}


} /* namespace ct */
