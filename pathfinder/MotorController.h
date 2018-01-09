#pragma once
#include <string>
#include <stdexcept>
#include <fstream>

namespace ct {

class Exception : public std::exception {};
class GpioInitException : public Exception {};
class PwmInitExportException : public Exception {};
class PwmInitiEnableException : public Exception {};
class PwmInitPeriodException : public Exception {};

class MotorController {

public:
	MotorController(int nPwm, int enaPin, int dirPin);
	virtual ~MotorController();

	void roll();
	void forward(int power);

private:
	std::ofstream initPwm(int nPwm) const;
	void deinitPwm(int nPwm) const;
	std::ofstream initGpio(int gpioPin) const;
	void deinitGpio(int gpioPin) const;

	int _nPwm;
	int _enaPin;
	int _dirPin;

	std::ofstream _pwmStream;
	std::ofstream _enaStream;
	std::ofstream _dirStream;
};
} // namespace ct
