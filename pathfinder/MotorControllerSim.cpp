#include "MotorControllerSim.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define BASE_GPIO (458)
#define PWM_PERIOD_NS (500000)
#define POWER_MAX (10)

namespace ct
{
MotorControllerSim::MotorControllerSim()
{
}

MotorControllerSim::~MotorControllerSim()
{
	roll();
}

void MotorControllerSim::roll()
{
	std::cout << "roll" << std::endl;
}

void MotorControllerSim::forward(int power)
{
	std::cout << "forward " << power << std::endl;
}

void MotorControllerSim::back(int power)
{
	std::cout << "back " << power << std::endl;
}
} // namespace ct
