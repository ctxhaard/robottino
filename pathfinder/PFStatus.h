
#pragma once

namespace ct {

class Pathfinder;

class PFStatus
{
public:
	void onFrontSensor(Pathfinder& self, int mm);
	void onLeftSensor(Pathfinder& self, int mm);
	void onRightSensor(Pathfinder& self, int mm);
};
} // namespace ct
