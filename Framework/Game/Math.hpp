#pragma once

#include "Framework/Game/FixedPoint.hpp"
#include "Framework/serialize/binary_archive.hpp"

#include "glm/glm.hpp"
#include "glm/detail/type_quat.hpp"

using vec2 = glm::vec<2, fixed16, glm::defaultp>;
using vec3 = glm::vec<3, fixed16, glm::defaultp>;
using vec4 = glm::vec<4, fixed16, glm::defaultp>;
using mat3 = glm::mat<3, 3, fixed16, glm::defaultp>;
using mat4 = glm::mat<4, 4, fixed16, glm::defaultp>;
using quat = glm::qua<fixed16, glm::defaultp>;

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
}

namespace serialize
{
	template<class Archive,glm::length_t N, class T, glm::qualifier Q>
	void Save(Archive& ar,const glm::vec<N,T,Q>& v)
	{
		for (glm::length_t i = 0; i < N; ++i)
		{
			ar(v[i]);
		}
	}

	template<class Archive, glm::length_t N, class T, glm::qualifier Q>
	void Load(Archive& ar,glm::vec<N, T, Q>& v)
	{
		for (glm::length_t i = 0; i < N; ++i)
		{
			ar(v[i]);
		}
	}
}