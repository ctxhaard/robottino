#pragma once
#include <string>
#include <stdexcept>
#include <fstream>

namespace ct {
class IMotorController {

public:
	virtual	void roll() = 0;
	virtual	void forward(int power) = 0;
	virtual	void back(int power) = 0;
};

class Exception : public std::exception {};
class GpioInitException : public Exception {};
class PwmInitExportException : public Exception {};
class PwmInitiEnableException : public Exception {};
class PwmInitPeriodException : public Exception {};

class MotorController : public IMotorController{

public:
	MotorController(int nPwm, int enaPin, int dirPin);
	virtual ~MotorController();

	void roll();
	void forward(int power);
	void back(int power);

private:
	std::string initPwm(int nPwm) const;
	void deinitPwm(int nPwm) const;
	std::string initGpio(int gpioPin) const;
	void deinitGpio(int gpioPin) const;

	int _nPwm;
	int _enaPin;
	int _dirPin;

	std::string _pwmDutyPath;
	std::string _enaPath;
	std::string _dirPath;
};
} // namespace ct
