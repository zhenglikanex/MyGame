#pragma once

#include "Framework/Game/LogService.hpp"

class UnityLogService : public LogService
{
public:
	void Info(std::string_view view) const override;
	void Warning(std::string_view view) const override;
	void Error(std::string_view view) const override;
};