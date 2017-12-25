#pragma once

#include <string>
#include <future>

namespace ct {
class ProximitySensor
{
public:
	ProximitySensor(std::string devPath);

	std::future<int> acquire(std::function<void(int)>);
private:
	std::string _devPath;
};
} // namespace ct
