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

namespace ct {

Pathfinder::Pathfinder(std::unique_ptr<MotorController> &&leftMotor, std::unique_ptr<MotorController> &&rightMotor, std::unique_ptr<ProximitySensor> &&frontSensor)
	: _ml{ std::move(leftMotor) }
	, _mr{ std::move(rightMotor) }
	, _sf{ std::move(frontSensor) }
	, _status{ std::make_unique<PFStatus>()  }
	, _display{ "/dev/lcd0" }
{
	// NOTE: devo usare una move() perché una r-value reference è un lvalue (così ho trovato scritto...)
}

int Pathfinder::run() {
	auto fs = _sf->acquire([this](int mm) {
			_display.seekp(16 + 5);
			_display << (mm / 10);
			_status->onFrontSensor(*this, mm);
			});

	auto ls = _sl->acquire([this](int mm) {
			_display.seekp(16);
			_display << (mm / 10);
			_status->onLeftSensor(*this, mm);
			});

	auto rs = _sr->acquire([this](int mm) {
			_display.seekp(16 + 10);
			_display << (mm / 10);
			_status->onRightSensor(*this, mm);
			});

	fs.get();
	ls.get();
	rs.get();
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


} /* namespace ct */
