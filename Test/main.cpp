#pragma once

#include <iostream>
#include <chrono>
#include <string>

#include "3rdparty/include/glm/glm.hpp"
#include "3rdparty/include/glm/gtc/quaternion.hpp"


#include "Kanex.hpp"

namespace kanex
{

	inline void Save(BinaryOutputArchive& ar, const glm::quat& q)
	{

	}

	inline void Load(BinaryInputArchive& ar, const glm::quat& q)
	{

	}

}

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
	decltype(Load(std::declval<kanex::BinaryInputArchive>(), std::declval<glm::quat>()), std::true_type()) type_value;

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
	return 0;
}