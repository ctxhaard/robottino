#include "MotorController.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define BASE_GPIO (458)
#define PWM_PERIOD_NS (500000)
#define PWM_DUTYCYCLE_MIN_NS (250000)
#define POWER_MAX (10)

namespace ct
{
MotorController::MotorController(int nPwm, int enaPin, int dirPin)
	: _nPwm{nPwm}, _enaPin{enaPin}, _dirPin{dirPin}
{
	_pwmDutyPath = initPwm(nPwm);
	_enaPath = initGpio(enaPin);
	_dirPath = initGpio(dirPin);
}

MotorController::~MotorController()
{
	roll();
	deinitPwm(_nPwm);
	deinitGpio(_enaPin);
	deinitGpio(_dirPin);
}

void MotorController::roll()
{
	std::cout << "roll" << std::endl;
	std::ofstream pwmStream{ _pwmDutyPath };
	std::ofstream enaStream{ _enaPath };
	pwmStream << "0";
	enaStream << "0";
}

void MotorController::brake()
{
	std::cout << "brake" << std::endl;
	std::ofstream pwmStream{ _pwmDutyPath };
	std::ofstream enaStream{ _enaPath };
	std::ofstream dirStream{ _dirPath };
	dirStream << "0";
	pwmStream << "0";
	enaStream << "1";
}

void MotorController::forward(int power)
{
	std::cout << "forward " << power << std::endl;
	std::ofstream pwmStream{ _pwmDutyPath };
	std::ofstream enaStream{ _enaPath };
	std::ofstream dirStream{ _dirPath };
	dirStream << "0";
	pwmStream << power2pwm(power);
	enaStream << "1";
}

void MotorController::back(int power)
{
	std::cout << "back " << power << std::endl;
	std::ofstream pwmStream{ _pwmDutyPath };
	std::ofstream enaStream{ _enaPath };
	std::ofstream dirStream{ _dirPath };
	dirStream << "1";
	pwmStream << power2pwm(power);
	enaStream << "1";
}
std::string MotorController::initPwm(int nPwm) const
{
	std::ofstream outf{"/sys/class/pwm/pwmchip0/export"};
	if (!outf.is_open()) { throw PwmInitExportException(); }
	outf << nPwm;
	outf.close();
	
	std::ostringstream oss;
        oss << "/sys/class/pwm/pwmchip0/pwm" << nPwm << "/period";
	outf.open(oss.str());
	std::cout << "opening " << oss.str() << std::endl;
	if (!outf.is_open()) { throw PwmInitPeriodException(); }
	outf << PWM_PERIOD_NS;
	outf.close();

	oss.clear();
	oss.str("");
	oss << "/sys/class/pwm/pwmchip0/pwm";
	oss << nPwm << "/enable";
	std::cout << "opening " << oss.str() << std::endl;
	outf.open(oss.str());
	if (!outf.is_open()) { throw PwmInitiEnableException(); }
	outf << 1;
	outf.close();

	oss.clear();
	oss.str("");
	oss << "/sys/class/pwm/pwmchip0/pwm" << nPwm << "/duty_cycle";

	return oss.str().c_str();
}

void MotorController::deinitPwm(int nPwm) const 
{
	std::ofstream outf{"/sys/class/pwm/pwmchip0/unexport"};
	if (outf.is_open()) {
		outf << nPwm;
		outf.close();
	}
}

std::string MotorController::initGpio(int gpioPin) const
{
	std::ofstream outf{"/sys/class/gpio/export"};
	if (!outf.is_open()) { throw GpioInitException();  }
	outf << (BASE_GPIO + gpioPin);
	outf.close();
	
	std::ostringstream oss;
	oss << "/sys/class/gpio/gpio" << (BASE_GPIO + gpioPin) << "/direction";
	outf.open(oss.str());
	if (!outf.is_open()) { throw GpioInitException(); }
	outf << "out";
	outf.close();
	
	oss.clear();
	oss.str("");
	oss << "/sys/class/gpio/gpio" << (BASE_GPIO + gpioPin) << "/value";
	return oss.str().c_str();
}

void MotorController::deinitGpio(int gpioPin) const
{
	std::ofstream outf{"/sys/class/gpio/unexport"};
	if (!outf.is_open()) { return; }
	outf << (BASE_GPIO + gpioPin);
	outf.close();
}

int MotorController::power2pwm(int power) const
{
	power = (power > POWER_MAX ? POWER_MAX : power);
	return (PWM_DUTYCYCLE_MIN_NS + (power * (PWM_PERIOD_NS - PWM_DUTYCYCLE_MIN_NS) / POWER_MAX));
}
} // namespace ct
