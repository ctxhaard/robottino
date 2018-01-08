#pragma once
#include <fstream>

namespace ct {

class Pathfinder;
class MotorController;

class PFStatus
{
public:
	PFStatus(Pathfinder &p, const char *name);
	virtual ~PFStatus();
	virtual void onFrontSensor(Pathfinder& self, int mm);
	virtual void onLeftSensor(Pathfinder& self, int mm);
	virtual void onRightSensor(Pathfinder& self, int mm);

protected:
	MotorController& getMotorLeft() const;
	MotorController& getMotorRight() const;
	std::ofstream& getDisplay();
	
protected:
	Pathfinder &_self;
};

// ---------------------------------------------------------------------------
class PFStatusRolling: public PFStatus
{
public:
	PFStatusRolling(Pathfinder &p);
	~PFStatusRolling();
	void onFrontSensor(Pathfinder& self, int mm);
	void onLeftSensor(Pathfinder& self, int mm);
	void onRightSensor(Pathfinder& self, int mm);
};
} // namespace ct
