#pragma once

#include <future>
#include "ProximitySensor.h"

namespace ct {
class ProximitySensorSim : public IProximitySensor
{
public:
	ProximitySensorSim();

	std::future<int> acquire(std::function<void(int)>) override;
	int getMm() override;
	bool hasNewMeas() const override;
private:
	void setMm(int mm);
	int _mm;
	bool _newMeas;
};
} // namespace ct
