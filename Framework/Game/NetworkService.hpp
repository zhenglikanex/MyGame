#pragma once

#include <functional>
#include <vector>
#include <any>

#include "Framework/Game/Service.hpp"

class NetworkService : public Service
{
public:
	virtual ~NetworkService() = 0 {}

	virtual std::unique_ptr<std::tuple<std::string, std::vector<uint8_t>>> Recv() const = 0;
	virtual void Send(std::string_view name, std::vector<uint8_t>&& data) const = 0;
};