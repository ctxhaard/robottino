#pragma once
#include <fstream>
#include <mutex>

namespace ct {

class Pathfinder;
class MotorController;
class ProximitySensor;

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
	ProximitySensor& getSensorLeft() const;
	ProximitySensor& getSensorRight() const;
	ProximitySensor& getSensorFront() const;
	std::ofstream& getDisplay();
	std::mutex& getDisplayMutex();
	void setStatus(PFStatus *newStatus);
	
protected:
	Pathfinder &_self;
};

// ---------------------------------------------------------------------------
class PFStatusRolling: public PFStatus
{
public:
	PFStatusRolling(Pathfinder &p);
	~PFStatusRolling();
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusForward: public PFStatus
{
public:
	PFStatusForward(Pathfinder &p);
	~PFStatusForward();
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};
 
// ---------------------------------------------------------------------------
class PFStatusBack: public PFStatus
{
public:
	PFStatusBack(Pathfinder &p);
	~PFStatusBack();
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusRotateLeft: public PFStatus
{
public:
	PFStatusRotateLeft(Pathfinder &p);
	~PFStatusRotateLeft();
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusRotateRight: public PFStatus
{
public:
	PFStatusRotateRight(Pathfinder &p);
	~PFStatusRotateRight();
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusTurnLeft: public PFStatus
{
public:
	PFStatusTurnLeft(Pathfinder &p);
	~PFStatusTurnLeft();
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusTurnRight: public PFStatus
{
public:
	PFStatusTurnRight(Pathfinder &p);
	~PFStatusTurnRight();
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};
} // namespace ct
