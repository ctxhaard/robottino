#pragma once
#include <string>
#include <stdexcept>

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
private:
	std::string initPwm(int nPwm) const;
	void deinitPwm(int nPwm) const;
	std::string initGpio(int gpioPin) const;
	void deinitGpio(int gpioPin) const;

	int _nPwm;
	int _enaPin;
	int _dirPin;

	std::string _pwmPath;
	std::string _enaPath;
	std::string _dirPath;
};
} // namespace ct
