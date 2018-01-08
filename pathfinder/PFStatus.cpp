#include "PFStatus.h"
#include "Pathfinder.h"
#include "MotorController.h"
#include "ProximitySensor.h"

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
// TODO: implement
}

PFStatusForward::~PFStatusForward()
{}

void PFStatusForward::onFrontSensor(Pathfinder& self, int mm)
{
// TODO: implement

}

void PFStatusForward::onLeftSensor(Pathfinder& self, int mm)
{
// TODO: implement

}

void PFStatusForward::onRightSensor(Pathfinder& self, int mm)
{
// TODO: implement

}
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
PFStatusBack::PFStatusBack(Pathfinder &p)
	: PFStatus(p, "back")
{
// TODO: implement
}

PFStatusBack::~PFStatusBack()
{}

void PFStatusBack::onFrontSensor(Pathfinder& self, int mm)
{
// TODO: implement

}

void PFStatusBack::onLeftSensor(Pathfinder& self, int mm)
{
// TODO: implement

}

void PFStatusBack::onRightSensor(Pathfinder& self, int mm)
{
// TODO: implement

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
{}

void PFStatusRotateLeft::onFrontSensor(Pathfinder& self, int mm)
{
// TODO: implement

}

void PFStatusRotateLeft::onLeftSensor(Pathfinder& self, int mm)
{
// TODO: implement

}

void PFStatusRotateLeft::onRightSensor(Pathfinder& self, int mm)
{
// TODO: implement

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

// TODO: implement
}

void PFStatusRotateRight::onLeftSensor(Pathfinder& self, int mm)
{
// TODO: implement

}

void PFStatusRotateRight::onRightSensor(Pathfinder& self, int mm)
{
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
// TODO: implement

}

void PFStatusTurnLeft::onLeftSensor(Pathfinder& self, int mm)
{
// TODO: implement

}

void PFStatusTurnLeft::onRightSensor(Pathfinder& self, int mm)
{
// TODO: implement

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
