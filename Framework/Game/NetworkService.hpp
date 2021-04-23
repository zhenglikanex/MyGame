#pragma once

#include <functional>

#include "Framework/Game/Service.hpp"

class NetworkService : public Service
{
public:
	virtual void Request(std::string_view name, std::vector<uint8_t>&& data,
		const std::function<void(std::vector<uint8_t>&& data)> & = nullptr) = 0;

	virtual void Send(std::string_view name, std::vector<uint8_t>&& data,
		const std::function<void(std::vector<uint8_t>&& data)> & = nullptr) = 0;
};