#include "PFStatus.h"
#include "Pathfinder.h"
#include "MotorController.h"

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
// if mm < 50 => back
// else if mm < 100
// 	rotate left or right (based on the bigger lateral distance)
// else
//	forward
}

void PFStatusRolling::onLeftSensor(Pathfinder& self, int mm)
{
// TODO: if mm < 80 & front acquired => rotate left

}

void onRightSensor(Pathfinder& self, int mm)
{
// TODO: if mm < 80 & front acquired => rotate right
}
} // namespace ct
