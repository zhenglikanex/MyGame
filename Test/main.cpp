#pragma once

#include "Framework/Game/Json.hpp"
#include "rapidjson/document.h"
#include <iostream>
#include "Framework/Game/Data/AnimationConfig.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/Fmt.hpp"
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>

#include "Framework/Game/Math.hpp"
#include "Framework/Game/Component/Attributes.hpp"

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
	glm::radians(vec3(0, 0, 0));
	quat q1(vec3(0, 90, 0));
	quat q(fixed16(0), fixed16(0.85), fixed16(0), fixed16(0.52));
	auto v = vec3(0, 0, 1) * q;
	auto v2 = vec3(0, 0, 1) * q1;
	

	fixed16 min = std::numeric_limits<fixed16>::min();
	fixed16 max = std::numeric_limits<fixed16>::max();
	auto fvmin = static_cast<float>(min);
	auto fv = static_cast<float>(max);
	

	std::string s1 =  fmt::format("this is {:.1f}", fixed16(0.16));
	std::string s = fmt::format("{:.1f}", 0.54f);



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
	
	
	{
		AutoTimer t("parse json");
		nlohmann::json j = nlohmann::json::parse(data);
		auto anim_info = j.get<AnimationInfo>();
	}

	return 0;
}