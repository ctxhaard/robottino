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
#include <condition_variable>

namespace ct {

Pathfinder::Pathfinder(std::unique_ptr<IMotorController> &&leftMotor, std::unique_ptr<IMotorController> &&rightMotor, std::unique_ptr<IProximitySensor> &&frontSensor)
	: _ml{ std::move(leftMotor) }
	, _mr{ std::move(rightMotor) }
	, _sf{ std::move(frontSensor) }
	, _display{ "/dev/lcd0" }
	, _stopRequested{false}
{
	// NOTE: devo usare una move() perché una r-value reference è un lvalue (così ho trovato scritto...)
}

int Pathfinder::run() {
	std::mutex m;
	std::condition_variable cv;
	bool ready = false;

	_stopRequested = false;
	_status = std::make_unique<PFStatusRolling>(*this); 
	auto fs = _sf->acquire([&](int mm) {
				{
					std::unique_lock<std::mutex> lk(m);
					ready = true;
				}
				//std::cout << "notify" << std::endl;
				cv.notify_one();	
				std::lock_guard<std::mutex> lock(_displayMutex);
				_display.seekp(16 + 5);
				_display << std::left << std::setw(5) << std::setfill('.') << (mm / 10);
			});

	auto ls = _sl->acquire([&](int mm) {
				{
					std::unique_lock<std::mutex> lk(m);
					ready = true;
				}
				//std::cout << "notify" << std::endl;
				cv.notify_one();	
				std::lock_guard<std::mutex> lock(_displayMutex);
				_display.seekp(16);
				_display << std::left << std::setw(5) << std::setfill('.') << (mm / 10);
			});

	auto rs = _sr->acquire([&](int mm) {
				{
					std::unique_lock<std::mutex> lk(m);
					ready = true;
				}
				//std::cout << "notify" << std::endl;
				cv.notify_one();	
				std::lock_guard<std::mutex> lock(_displayMutex);
				_display.seekp(16 + 10);
				_display << std::right << std::setw(5) << std::setfill('.') << (mm / 10);
			});

	while(!_stopRequested) {
		{
			std::unique_lock<std::mutex> lk(m);
			cv.wait(lk, [&ready]{ return ready; });
			ready = false;
		}
//		std::cout << std::setw(6) << _sl->getMm() << "|"
//				<< std::setw(6) << _sf->getMm() << "|"
//				<< std::setw(6) << _sr->getMm() << std::endl;
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

	_ml->roll();
	_mr->roll();
	_sf->stop();
	_sl->stop();
	_sr->stop();

	fs.get();
	ls.get();
	rs.get();

	return 0;
}

void Pathfinder::stop()
{
	_stopRequested = true;
	std::cout << "PATHFINDER GOT A STOP REQUEST\n";
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
	std::this_thread::sleep_for( std::chrono::milliseconds( 500 ));
	_status.reset(newStatus);
	_status->begin();
}


} /* namespace ct */
