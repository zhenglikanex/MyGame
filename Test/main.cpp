#pragma once

#include "Framework/Game/Json.hpp"
#include "rapidjson/document.h"
#include <iostream>
#include "Framework/Game/Data/AnimationConfig.hpp"
#include "Framework/Game/FileService.hpp"
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>

class Timer
{
public:
	Timer()
	{
		start_ = std::chrono::system_clock::now();
	}

	double Elapsed()
	{
		return std::chrono::duration<double>(std::chrono::system_clock::now() - start_).count();
	}
private:
	std::chrono::system_clock::time_point start_;
};

class AutoTimer
{
public:
	AutoTimer() = default;
	AutoTimer(const std::string& _name) : name(_name) { }
	~AutoTimer() {
		std::cout << name << ":" << timer.Elapsed() * 1000 << " ms" << std::endl;
	}

	Timer timer;
	std::string name;
};

int main()
{
	
	
	AutoTimer t("fopen");
	FILE *f = NULL;
	long len = 0;

	/* open in read binary mode */
	f = fopen("Hero.json", "rb");
	/* get the length */
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* data = (char*)malloc(len + 1);

	fread(data, 1, len, f);
	data[len] = '\0';
	fclose(f);
	
	nlohmann::json j = nlohmann::json::parse(data);
	{
		AutoTimer t("parse json");
		auto anim_info = j.get<AnimationInfo>();
	}

	return 0;
}