#pragma once

#include <string>
#include <future>

namespace ct {
class IProximitySensor
{
public:
	virtual	std::future<int> acquire(std::function<void(int)>) = 0;
	virtual void stop() = 0;
	virtual	int getMm() = 0;
	virtual	bool hasNewMeas() const = 0;
};

class ProximitySensor : public IProximitySensor
{
public:
	ProximitySensor(std::string devPath);

	std::future<int> acquire(std::function<void(int)>) override;
	void stop() override;
	int getMm() override;
	bool hasNewMeas() const override;
private:
	void setMm(int mm);

	std::string _devPath;
	int _mm;
	bool _newMeas;
	bool _stopRequested;
};
} // namespace ct
