#include "MotorController.h"
#include <fstream>
#include <sstream>

#define BASE_GPIO (458)
#define PWM_PERIOD_NS (500000)

namespace ct
{
MotorController::MotorController(int nPwm, int enaPin, int dirPin)
	: _nPwm{nPwm}, _enaPin{enaPin}, _dirPin{dirPin}
{
	_pwmPath = initPwm(nPwm);
	_enaPath = initGpio(enaPin);
	_dirPath = initGpio(dirPin);
}

MotorController::~MotorController()
{
	deinitPwm(_nPwm);
	deinitGpio(_enaPin);
	deinitGpio(_dirPin);
}
std::string MotorController::initPwm(int nPwm) const
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
	return oss.str();
}

void MotorController::deinitPwm(int nPwm) const
{
	std::ofstream outf{"/sys/class/pwm/pwmchip0/unexport"};
	if (!outf.is_open()) { return; }
	outf << nPwm;
	outf.close();
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
	return oss.str();
}

void MotorController::deinitGpio(int gpioPin) const
{
	std::ofstream outf{"/sys/class/gpio/unexport"};
	if (!outf.is_open()) { return; }
	outf << (BASE_GPIO + gpioPin);
	outf.close();
}
} // namespace ct
