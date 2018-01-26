#include "PFStatus.h"
#include "Pathfinder.h"
#include "MotorController.h"
#include "ProximitySensor.h"
#include <iostream>
#include <mutex>
#include <iomanip>

namespace ct
{
PFStatus::PFStatus(Pathfinder& p, const char *name)
	: _self{ p }, _name{ name }
{
}

PFStatus::~PFStatus()
{
}

void PFStatus::onFrontSensor(Pathfinder& self, int mm)
{

}

void PFStatus::onLeftSensor(Pathfinder& self, int mm)
{

}

void PFStatus::onRightSensor(Pathfinder& self, int mm)
{

}

IMotorController& PFStatus::getMotorLeft() const 
{
	return *_self._ml.get();
}

IMotorController& PFStatus::getMotorRight() const
{
	return *_self._mr.get();
}

std::ofstream& PFStatus::getDisplay()
{
	return _self._display;
}

std::mutex& PFStatus::getDisplayMutex()
{
	return _self._displayMutex;
}

std::mutex& PFStatus::getStatusMutex()
{
	return _self._statusMutex;
}

IProximitySensor& PFStatus::getSensorLeft() const
{
	return *_self._sl.get();
}

IProximitySensor& PFStatus::getSensorRight() const
{
	return *_self._sr.get();

}

IProximitySensor& PFStatus::getSensorFront() const
{
	return *_self._sf.get();
}

void PFStatus::setStatus(PFStatus *newStatus)
{
	{
		std::lock_guard<std::mutex> displayLock(getDisplayMutex());
		getDisplay().seekp(0)  << std::left 
			<< std::setw(15) << std::setfill('.') 
			<< newStatus->_name << std::flush;
	}
	std::cout << "==> status: " << newStatus->_name << std::endl;
	_self.setStatus( newStatus );
}

void PFStatus::begin()
{

}

void PFStatus::end()
{
	getMotorLeft().roll();
	getMotorRight().roll();
}

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusRolling::PFStatusRolling(Pathfinder& p)
	: PFStatus(p, "rolling") 
{
}

void PFStatusRolling::begin() 
{
	getMotorLeft().roll();
	getMotorRight().roll();
}

void PFStatusRolling::onFrontSensor(Pathfinder& self, int mm)
{
	PFStatus::onFrontSensor(self, mm);
	if (mm < FRONT_MM_MIN) {
		setStatus(new PFStatusBack(self));
	} else if (mm < FRONT_MM_ALERT) {
		if(getSensorLeft().getMm() > getSensorRight().getMm()) {
			setStatus(new PFStatusRotateLeft(self));
		} else {
			setStatus(new PFStatusRotateRight(self));
		}
	} else {
		setStatus(new PFStatusForward(self));	
	}
}

void PFStatusRolling::onLeftSensor(Pathfinder& self, int mm)
{
	PFStatus::onLeftSensor(self, mm);
	if (mm < LATERAL_MM_MIN && getSensorFront().getMm() >= 0) {
		setStatus(new PFStatusTurnRight(self));
	}
}

void PFStatusRolling::onRightSensor(Pathfinder& self, int mm)
{
	PFStatus::onRightSensor(self, mm);
	if (mm < LATERAL_MM_MIN && getSensorFront().getMm() >= 0) {
		setStatus(new PFStatusTurnLeft(self));
	}
}
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusForward::PFStatusForward(Pathfinder &p)
	: PFStatus(p, "forward")
{
}

void PFStatusForward::begin()
{
	getMotorLeft().forward(10);
	getMotorRight().forward(10);
}

void PFStatusForward::end()
{
	getMotorLeft().brake();
	getMotorRight().brake();
}

void PFStatusForward::onFrontSensor(Pathfinder& self, int mm)
{
	PFStatus::onFrontSensor(self, mm);
	if (mm < FRONT_MM_MIN) {
		setStatus(new PFStatusBack(self));
	} else if (mm < FRONT_MM_ALERT) {
		if(getSensorLeft().getMm() > getSensorRight().getMm()) {
			setStatus(new PFStatusRotateLeft(self));
		} else {
			setStatus(new PFStatusRotateRight(self));
		}
	}
}

void PFStatusForward::onLeftSensor(Pathfinder& self, int mm)
{
	PFStatus::onLeftSensor(self, mm);
	if (mm < LATERAL_MM_MIN && getSensorFront().getMm() >= 0) {
		setStatus(new PFStatusTurnRight(self));
	}
}

void PFStatusForward::onRightSensor(Pathfinder& self, int mm)
{
	PFStatus::onRightSensor(self, mm);
	if (mm < LATERAL_MM_MIN && getSensorFront().getMm() >= 0) {
		setStatus(new PFStatusTurnLeft(self));
	}
}
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusBack::PFStatusBack(Pathfinder &p)
	: PFStatus(p, "back")
{
}

void PFStatusBack::begin()
{
	getMotorLeft().back(5);
	getMotorRight().back(5);
}

void PFStatusBack::end()
{
	getMotorLeft().brake();
	getMotorRight().brake();
}

void PFStatusBack::onFrontSensor(Pathfinder& self, int mm)
{
	PFStatus::onFrontSensor(self, mm);
	if (mm >= FRONT_MM_ALERT) {
		setStatus(new PFStatusRolling(self));
	} 
}

void PFStatusBack::onLeftSensor(Pathfinder& self, int mm)
{
	PFStatus::onLeftSensor(self, mm);
	if (mm < LATERAL_MM_MIN && getSensorFront().getMm() >= 0) {
		setStatus(new PFStatusRotateRight(self));
	}
}

void PFStatusBack::onRightSensor(Pathfinder& self, int mm)
{
	PFStatus::onRightSensor(self, mm);
	if (mm < LATERAL_MM_MIN && getSensorFront().getMm() >= 0) {
		setStatus(new PFStatusRotateLeft(self));
	}
}
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusRotateLeft::PFStatusRotateLeft(Pathfinder &p)
	: PFStatus(p, "rotate-left")
{
}

void PFStatusRotateLeft::begin()
{
	getMotorLeft().back(5);
	getMotorRight().forward(5);
}

void PFStatusRotateLeft::onFrontSensor(Pathfinder& self, int mm)
{
	PFStatus::onFrontSensor(self, mm);	
	if (mm >= FRONT_MM_OK) {
		setStatus(new PFStatusForward(self));
	}
}

void PFStatusRotateLeft::onLeftSensor(Pathfinder& self, int mm)
{
	PFStatus::onLeftSensor(self, mm);
}

void PFStatusRotateLeft::onRightSensor(Pathfinder& self, int mm)
{
	PFStatus::onRightSensor(self, mm);
}

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusRotateRight::PFStatusRotateRight(Pathfinder &p)
	: PFStatus(p, "rotate-right")
{
}

void PFStatusRotateRight::begin()
{
	getMotorLeft().forward(5);
	getMotorRight().back(5);
}

void PFStatusRotateRight::onFrontSensor(Pathfinder& self, int mm)
{
	PFStatus::onFrontSensor(self, mm);	
	if (mm >= FRONT_MM_OK) {
		setStatus(new PFStatusForward(self));
	}
}

void PFStatusRotateRight::onLeftSensor(Pathfinder& self, int mm)
{
	PFStatus::onLeftSensor(self, mm);
}

void PFStatusRotateRight::onRightSensor(Pathfinder& self, int mm)
{
	PFStatus::onRightSensor(self, mm);
}
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusTurnLeft::PFStatusTurnLeft(Pathfinder &p)
	: PFStatus(p, "turn-left")
{
}

void PFStatusTurnLeft::begin()
{
	getMotorLeft().roll();
	getMotorRight().forward(10);
}

void PFStatusTurnLeft::onFrontSensor(Pathfinder& self, int mm)
{
	PFStatus::onFrontSensor(self, mm);	
	if (mm >= FRONT_MM_OK 
		&& (getSensorRight().isNotDecreasing() || getSensorRight().getMm() >= LATERAL_MM_GOOD)) {
		setStatus(new PFStatusForward(self));
	}
}

void PFStatusTurnLeft::onLeftSensor(Pathfinder& self, int mm)
{
	PFStatus::onLeftSensor(self, mm);
}

void PFStatusTurnLeft::onRightSensor(Pathfinder& self, int mm)
{
	PFStatus::onRightSensor(self, mm);
}
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusTurnRight::PFStatusTurnRight(Pathfinder &p)
	: PFStatus(p, "turn-right")
{
}

void PFStatusTurnRight::begin()
{
	getMotorLeft().forward(10);
	getMotorRight().roll();
}

void PFStatusTurnRight::onFrontSensor(Pathfinder& self, int mm)
{
	PFStatus::onFrontSensor(self, mm);	
	if (mm >= FRONT_MM_OK 
		&& (getSensorLeft().isNotDecreasing() || getSensorLeft().getMm() >= LATERAL_MM_GOOD)) {
		setStatus(new PFStatusForward(self));
	}
}

void PFStatusTurnRight::onLeftSensor(Pathfinder& self, int mm)
{
// TODO: implement

}

void PFStatusTurnRight::onRightSensor(Pathfinder& self, int mm)
{
// TODO: implement
}
} // namespace ct
