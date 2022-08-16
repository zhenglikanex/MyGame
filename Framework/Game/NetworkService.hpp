#pragma once


#include <vector>
#include <optional>

#include "Framework/Game/Service.hpp"

class NetworkService : public Service
{
public:
	virtual std::optional<std::vector<uint8_t>> Recv(const std::string& name) const = 0;
	virtual void Send(std::string_view name, std::vector<uint8_t>&& data) const = 0;
};