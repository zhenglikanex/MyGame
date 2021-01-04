#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <filesystem>

#include "Framework/Game/Service.hpp"


class FileService : public Service
{
public:
	virtual ~FileService() {}
	
	virtual std::string OpenFile(std::string_view file) const;

	void set_cur_path(std::string_view path) { cur_path_ = std::filesystem::path(path).string(); }
	const std::string& cur_path() const { return cur_path_; }
private:
	std::string cur_path_;
};