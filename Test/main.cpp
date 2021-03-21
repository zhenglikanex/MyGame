#pragma once

#include "Framework/Game/Json.hpp"
#include "rapidjson/document.h"
#include <iostream>
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/Fmt.hpp"
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>

#include "Framework/Game/Math.hpp"
#include "Framework/Game/Component/Attributes.hpp"
#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Collision.hpp"

struct Box
{
	glm::vec3 e;    // Positive halfwidth extents of OBB along each axis

	Box() = default;

	Box(const glm::vec3& _e)
		: e(_e)
	{
	}

};


int TestOBBOBB(Box& a, Box& b,const glm::mat4& t1,const glm::mat4& t2)
{
    float ra, rb;
    glm::mat3 R, AbsR;

    // Compute rotation matrix expressing b in a's coordinate frame
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            R[i][j] = glm::dot(t1[i], t2[j]);

    // Compute translation vector t
    glm::vec3 t = t2[3] - t1[3];
    // Bring translation into a's coordinate frame
    t = glm::mat3(t1) * t;
    //t = glm::vec3(glm::dot(t, glm::vec3(t1[0])), glm::dot(t, glm::vec3(t1[1])), glm::dot(t, glm::vec3(t1[2])));

    // Compute common subexpressions. Add in an epsilon term to
    // counteract arithmetic errors when two edges are parallel and
    // their cross product is (near) null (see text for details)
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            AbsR[i][j] = std::abs(R[i][j]) + std::numeric_limits<float>::epsilon();

    // Test axes L = A0, L = A1, L = A2
    for (int i = 0; i < 3; i++) {
        ra = a.e[i];
        rb = b.e[0] * AbsR[i][0] + b.e[1] * AbsR[i][1] + b.e[2] * AbsR[i][2];
        if (std::abs(t[i]) > ra + rb) return 0;
    }

    // Test axes L = B0, L = B1, L = B2
    for (int i = 0; i < 3; i++) {
        ra = a.e[0] * AbsR[0][i] + a.e[1] * AbsR[1][i] + a.e[2] * AbsR[2][i];
        rb = b.e[i];
        if (std::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return 0;
    }

    // Test axis L = A0 x B0
    ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
    rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];
    if (std::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return 0;

    // Test axis L = A0 x B1
    ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
    rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];
    if (std::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return 0;

    // Test axis L = A0 x B2
    ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
    rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];
    if (std::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return 0;

    // Test axis L = A1 x B0
    ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
    rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];
    if (std::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return 0;

    // Test axis L = A1 x B1
    ra = a.e[0] * AbsR[2][1] + a.e[2] * AbsR[0][1];
    rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0];
    if (std::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return 0;

    // Test axis L = A1 x B2
    ra = a.e[0] * AbsR[2][2] + a.e[2] * AbsR[0][2];
    rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0];
    if (std::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return 0;

    // Test axis L = A2 x B0
    ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
    rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
    if (std::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return 0;

    // Test axis L = A2 x B1
    ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
    rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];
    if (std::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return 0;

    // Test axis L = A2 x B2
    ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
    rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];
    if (std::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return 0;

    // Since no separating axis found, the OBBs must be intersecting
    return 1;
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
	Geometry collider1(OBB(vec3(0.5)));
	Geometry collider2(OBB(vec3(0.5)));
	
    Box box1(glm::vec3(0.5));
    Box box2(glm::vec3(0.5));

	auto r111 = TestBoxBox(collider1, collider2, glm::translate(glm::identity<mat4>(), vec3(0)),glm::translate(glm::identity<mat4>(), vec3(1.1)));
    auto r222 = TestOBBOBB(box1, box2, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0)), glm::translate(glm::identity<glm::mat4>(),glm::vec3(1.05)) * glm::rotate(glm::identity<glm::mat4>(),glm::radians(90.0f),glm::vec3(0,1.0,0)));

	auto mat = glm::translate(glm::identity<mat4>(), vec3(5, 6, 7)) * mat4(quat(vec3(60, 30, 50)));
	auto mat2 = mat4(quat(vec3(60, 30, 50))) * glm::translate(glm::identity<mat4>(), vec3(5, 6, 7));
	auto mat3 = mat4(quat(vec3(60, 30, 50)));
	mat3[3] = vec4(5, 6, 7,1);

	fixed16 v22;
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
	return 0;
}