#pragma once

#include <future>
#include "ProximitySensor.h"

namespace ct {
class ProximitySensorSim : public IProximitySensor
{
public:
	ProximitySensorSim();

	std::future<int> acquire(std::function<void(int)>) override;
	void stop() override;
	int getMm() override;
	bool hasNewMeas() const override;
	bool isNotDecreasing() const override;
private:
	void setMm(int mm);
	int _mm;
	bool _newMeas;
};
} // namespace ct
