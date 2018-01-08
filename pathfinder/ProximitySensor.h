#pragma once

#include <string>
#include <future>

namespace ct {
class ProximitySensor
{
public:
	ProximitySensor(std::string devPath);

	std::future<int> acquire(std::function<void(int)>);
	int getMm() const;
private:
	std::string _devPath;
	int _mm;
};
} // namespace ct
