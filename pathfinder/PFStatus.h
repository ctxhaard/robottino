#pragma once
#include <fstream>
#include <mutex>

namespace ct {

class Pathfinder;
class IMotorController;
class IProximitySensor;

class PFStatus
{

friend class Pathfinder;

public:
	PFStatus(Pathfinder &p, const char *name);
	virtual ~PFStatus();
	virtual void onFrontSensor(Pathfinder& self, int mm);
	virtual void onLeftSensor(Pathfinder& self, int mm);
	virtual void onRightSensor(Pathfinder& self, int mm);

protected:
	IMotorController& getMotorLeft() const;
	IMotorController& getMotorRight() const;
	IProximitySensor& getSensorLeft() const;
	IProximitySensor& getSensorRight() const;
	IProximitySensor& getSensorFront() const;
	std::ofstream& getDisplay();
	std::mutex& getDisplayMutex();
	void setStatus(PFStatus *newStatus);
	std::mutex& getStatusMutex();
	virtual void begin();
	virtual void end();
	
protected:
	Pathfinder &_self;
	std::string _name;
};

// ---------------------------------------------------------------------------
class PFStatusRolling: public PFStatus
{
public:
	PFStatusRolling(Pathfinder &p);
	void begin() override;
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusForward: public PFStatus
{
public:
	PFStatusForward(Pathfinder &p);
	void begin() override;
	void end() override;
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};
 
// ---------------------------------------------------------------------------
class PFStatusBack: public PFStatus
{
public:
	PFStatusBack(Pathfinder &p);
	void begin() override;
	void end() override;
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusRotateLeft: public PFStatus
{
public:
	PFStatusRotateLeft(Pathfinder &p);
	void begin() override;
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusRotateRight: public PFStatus
{
public:
	PFStatusRotateRight(Pathfinder &p);
	void begin() override;
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusTurnLeft: public PFStatus
{
public:
	PFStatusTurnLeft(Pathfinder &p);
	void begin() override;
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};

// ---------------------------------------------------------------------------
class PFStatusTurnRight: public PFStatus
{
public:
	PFStatusTurnRight(Pathfinder &p);
	void begin() override;
	void onFrontSensor(Pathfinder& self, int mm) override;
	void onLeftSensor(Pathfinder& self, int mm) override;
	void onRightSensor(Pathfinder& self, int mm) override;
};
} // namespace ct
