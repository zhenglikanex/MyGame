#pragma once

#include "Fmt.hpp"

#include "Framework/Game/FixedPoint.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/compatibility.hpp"

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat3 = glm::mat3;
using mat4 = glm::mat4;
using quat = glm::quat;

namespace glm
{
	using fpm::sin;
	using fpm::cos;
}

template<class T>
T zero()
{
	return T(0.0f);
}

inline vec3 ProjectOnPlane(vec3 vec, vec3 plane_normal)
{
	float sqr_mag = glm::dot(plane_normal, plane_normal);
	if (sqr_mag < std::numeric_limits<float>::epsilon())
	{
		return vec;
	}
	else
	{
		auto dot = glm::dot(vec, plane_normal);
		return vec3(vec.x - plane_normal.x * dot / sqr_mag,
			vec.y - plane_normal.y * dot / sqr_mag,
			vec.z - plane_normal.z * dot / sqr_mag);
		//return vec - plane_normal * dot / sqr_mag;
	}
}

inline vec3 Project(vec3 vector, vec3 on_normal)
{
	float sqr_mag = glm::dot(on_normal, on_normal);
	if (sqr_mag < std::numeric_limits<float>::epsilon())
	{
		return vector;
	}
	else
	{
		auto dot = glm::dot(vector, on_normal);
		return on_normal * dot / sqr_mag;
	}
}

inline quat LookRotation(vec3 view, vec3 up)
{
	return glm::quatLookAtLH(view, up);
}

inline vec3 Normalize(const vec3& v)
{
	auto length = glm::length(v);
	if (length < std::numeric_limits<float>::epsilon())
	{
		return vec3(0);
	}

	auto inversesqrt = 1 / length;
	return v * inversesqrt;
}

namespace glm
{
	template<glm::length_t N, class T, glm::qualifier Q>
	void from_json(const json& j, glm::vec<N, T, Q>& v)
	{
		std::string fileds[] = { "x","y","z","w" };
		for (int i = 0; i < N; ++i)
		{
			j.at(fileds[i]).get_to(v[i]);
		}
	}

	inline void from_json(const json& j, quat& q)
	{
		j.at("x").get_to(q.x);
		j.at("y").get_to(q.y);
		j.at("z").get_to(q.z);
		j.at("w").get_to(q.w);
	}

	template<glm::length_t col, glm::length_t row, class T, glm::qualifier Q>
	void from_json(const json& json, glm::mat<col, row, T, Q>& mat)
	{
		std::string str = json.dump();
		for (int i = 0; i < col; ++i)
		{
			for (int j = 0; j < row; ++j)
			{
				std::string key = fmt::format("m{0}{1}", i, j);
				json.at(key).get_to(mat[i][j]);
			}
		}
	}
}

namespace kanex
{

	template<class Archive, glm::length_t N, class T, glm::qualifier Q>
	void Save(Archive& ar, const glm::vec<N, T, Q>& v)
	{
		for (glm::length_t i = 0; i < N; ++i)
		{
			ar(v[i]);
		}
	}

	template<class Archive, glm::length_t N, class T, glm::qualifier Q>
	void Load(Archive& ar, glm::vec<N, T, Q>& v)
	{
		for (glm::length_t i = 0; i < N; ++i)
		{
			ar(v[i]);
		}
	}

	template<class Archive, glm::length_t N, class T, glm::qualifier Q>
	void Save(Archive& ar, const glm::mat<N, N, T, Q>& m)
	{
		for (glm::length_t i = 0; i < N; ++i)
		{
			for (glm::length_t j = 0; j < N; ++j)
			{
				ar(m[i][j]);
			}
		}
	}

	template<class Archive, glm::length_t N, class T, glm::qualifier Q>
	void Load(Archive& ar, glm::mat<N, N, T, Q>& m)
	{
		for (glm::length_t i = 0; i < N; ++i)
		{
			for (glm::length_t j = 0; j < N; ++j)
			{
				ar(m[i][j]);
			}
		}
	}

	template<class Archive, class T, glm::qualifier Q>
	void Serialize(Archive& ar, glm::qua<T, Q>& q)
	{
		ar(q.x, q.y, q.z, q.w);
	}
}