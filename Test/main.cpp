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
	auto mat = glm::translate(glm::identity<mat4>(), vec3(5, 6, 7)) * mat4(quat(vec3(60, 30, 50)));
	auto mat2 = mat4(quat(vec3(60, 30, 50))) * glm::translate(glm::identity<mat4>(), vec3(5, 6, 7));
	auto mat3 = mat4(quat(vec3(60, 30, 50)));
	mat3[3] = vec4(5, 6, 7,1);

	fixed16 v22;int
	fixed16 v = 0.01745329251994329576923690768489;
	auto a = fixed16(-180.0);
	float f1 = static_cast<float>(a);
	auto v1 = static_cast<float>(v * fixed16(-180.0));
	auto q2 = glm::qua(glm::vec3(0, glm::radians(180.0f), 0));
	auto r = glm::radians(-180.0);
	auto r1 = static_cast<float>(glm::radians(fixed16(-180.0f)));
	auto q1 = quat(vec3(fixed16(0), glm::radians(a), fixed16(0)));
	
	float y = static_cast<float>(q1.y);
	float w = static_cast<float>(q1.w);

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