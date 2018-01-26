#pragma once

#include "MotorController.h"

namespace ct {

class MotorControllerSim : public IMotorController {

public:
	MotorControllerSim();
	virtual ~MotorControllerSim();

	void roll() override;
	void brake() override;
	void forward(int power) override;
	void back(int power) override;
};
} // namespace ct
