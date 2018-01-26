#pragma once

#include <string>
#include <future>
#include <queue>

namespace ct {
class IProximitySensor
{
public:
	virtual	std::future<int> acquire(std::function<void(int)>) = 0;
	virtual void stop() = 0;
	virtual	int getMm() = 0;
	virtual	bool hasNewMeas() const = 0;
	virtual bool isNotDecreasing() const = 0;
};

class ProximitySensor : public IProximitySensor
{
public:
	ProximitySensor(std::string devPath);

	std::future<int> acquire(std::function<void(int)>) override;
	void stop() override;
	int getMm() override;
	void setMm(int mm); // public because of testing
	bool hasNewMeas() const override;
	bool isNotDecreasing() const override;
private:

	std::string _devPath;
	std::queue<int> _mm;
	bool _newMeas;
	bool _stopRequested;
};
} // namespace ct
