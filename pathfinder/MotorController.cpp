#include "MotorController.h"
#include <fstream>
#include <sstream>

#define BASE_GPIO (458)
#define PWM_PERIOD_NS (500000)
#define POWER_MAX (10)

namespace ct
{
MotorController::MotorController(int nPwm, int enaPin, int dirPin)
	: _nPwm{nPwm}, _enaPin{enaPin}, _dirPin{dirPin}
{
	_pwmStream = initPwm(nPwm);
	_enaStream = initGpio(enaPin);
	_dirStream = initGpio(dirPin);
}

MotorController::~MotorController()
{
	deinitPwm(_nPwm);
	deinitGpio(_enaPin);
	deinitGpio(_dirPin);

	if (_pwmStream.is_open()) { _pwmStream.close(); }
	if (_enaStream.is_open()) { _enaStream.close(); }
	if (_dirStream.is_open()) { _dirStream.close(); }
}

void MotorController::roll()
{
	_pwmStream << "0" << std::flush;
	_enaStream << "0" << std::flush;
}

void MotorController::forward(int power)
{
	power = (power > POWER_MAX ? POWER_MAX : power);
	_dirStream << "1" << std::flush;
	_pwmStream << (power * PWM_PERIOD_NS / POWER_MAX)  << std::flush;
	_enaStream << "1" << std::flush;
}

std::ofstream MotorController::initPwm(int nPwm) const
{
	std::ofstream outf{"/sys/class/pwm/pwmchip0/export"};
	if (!outf.is_open()) { throw PwmInitExportException(); }
	outf << nPwm;
	outf.close();
	
	std::ostringstream oss {"/sys/class/pwm/pwmchip0/pwm"};
	oss << nPwm << "/period";
	outf.open(oss.str());
	if (!outf.is_open()) { throw PwmInitPeriodException(); }
	outf << PWM_PERIOD_NS;
	outf.close();

	oss.clear();
	oss.str("");
	oss << "/sys/class/pwm/pwmchip0/pwm";
	oss << nPwm << "/enable";
	outf.open(oss.str());
	if (!outf.is_open()) { throw PwmInitiEnableException(); }
	outf << 1;
	outf.close();

	oss.clear();
	oss.str("");
	oss << "/sys/class/pwm/pwmchip0/pwm" << nPwm << "/duty_cycle";

	return std::ofstream(oss.str().c_str());
}

void MotorController::deinitPwm(int nPwm) const 
{
	std::ofstream outf{"/sys/class/pwm/pwmchip0/unexport"};
	if (outf.is_open()) {
		outf << nPwm;
		outf.close();
	}
}

std::ofstream MotorController::initGpio(int gpioPin) const
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
	return std::ofstream(oss.str().c_str());
}

void MotorController::deinitGpio(int gpioPin) const
{
	std::ofstream outf{"/sys/class/gpio/unexport"};
	if (!outf.is_open()) { return; }
	outf << (BASE_GPIO + gpioPin);
	outf.close();
}
} // namespace ct
