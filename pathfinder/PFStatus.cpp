#include "PFStatus.h"
#include "Pathfinder.h"
#include "MotorController.h"
#include "ProximitySensor.h"
#include <iostream>

namespace ct
{
PFStatus::PFStatus(Pathfinder& p, const char *name)
	: _self{ p }
{
	getDisplay() << name << std::flush;
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

MotorController& PFStatus::getMotorLeft() const 
{
	return *_self._ml.get();
}

MotorController& PFStatus::getMotorRight() const
{
	return *_self._mr.get();
}

std::ofstream &PFStatus::getDisplay()
{
	return _self._display;
}

ProximitySensor& PFStatus::getSensorLeft() const
{
	return *_self._sl.get();
}

ProximitySensor& PFStatus::getSensorRight() const
{
	return *_self._sr.get();

}

ProximitySensor& PFStatus::getSensorFront() const
{
	return *_self._sf.get();
}

void PFStatus::setStatus(PFStatus *newStatus)
{
	_self._status.reset(newStatus);
}

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusRolling::PFStatusRolling(Pathfinder& p)
	: PFStatus(p, "rolling") 
{
	getMotorLeft().roll();
	getMotorRight().roll();
}

PFStatusRolling::~PFStatusRolling()
{

}

void PFStatusRolling::onFrontSensor(Pathfinder& self, int mm)
{
	std::cout << __func__ << std::endl;
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
	getMotorLeft().forward(10);
	getMotorRight().forward(10);
}

PFStatusForward::~PFStatusForward()
{
	getMotorLeft().roll();
	getMotorRight().roll();
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
	getMotorLeft().back(5);
	getMotorRight().back(5);
}

PFStatusBack::~PFStatusBack()
{
	getMotorLeft().roll();
	getMotorRight().roll();
}

void PFStatusBack::onFrontSensor(Pathfinder& self, int mm)
{
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
// TODO: implement
}

PFStatusRotateLeft::~PFStatusRotateLeft()
{
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
// TODO: implement

}

void PFStatusRotateLeft::onRightSensor(Pathfinder& self, int mm)
{
// TODO: implement
	PFStatus::onRightSensor(self, mm);
}

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusRotateRight::PFStatusRotateRight(Pathfinder &p)
	: PFStatus(p, "rotate-right")
{
// TODO: implement
}

PFStatusRotateRight::~PFStatusRotateRight()
{}

void PFStatusRotateRight::onFrontSensor(Pathfinder& self, int mm)
{
	PFStatus::onFrontSensor(self, mm);	
// TODO: implement
}

void PFStatusRotateRight::onLeftSensor(Pathfinder& self, int mm)
{
	PFStatus::onLeftSensor(self, mm);
// TODO: implement
}

void PFStatusRotateRight::onRightSensor(Pathfinder& self, int mm)
{
	PFStatus::onRightSensor(self, mm);
// TODO: implement
}
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusTurnLeft::PFStatusTurnLeft(Pathfinder &p)
	: PFStatus(p, "turn-left")
{
// TODO: implement
}

PFStatusTurnLeft::~PFStatusTurnLeft()
{}

void PFStatusTurnLeft::onFrontSensor(Pathfinder& self, int mm)
{
	PFStatus::onFrontSensor(self, mm);	
// TODO: implement
}

void PFStatusTurnLeft::onLeftSensor(Pathfinder& self, int mm)
{
// TODO: implement
	PFStatus::onLeftSensor(self, mm);
}

void PFStatusTurnLeft::onRightSensor(Pathfinder& self, int mm)
{
// TODO: implement
	PFStatus::onRightSensor(self, mm);
}
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusTurnRight::PFStatusTurnRight(Pathfinder &p)
	: PFStatus(p, "turn-right")
{
// TODO: implement
}

PFStatusTurnRight::~PFStatusTurnRight()
{}

void PFStatusTurnRight::onFrontSensor(Pathfinder& self, int mm)
{
// TODO: implement

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
