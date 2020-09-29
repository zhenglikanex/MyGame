#pragma once

#include "Framework/Game/FixedPoint.hpp"
#include "Framework/serialize/binary_archive.hpp"
#include "glm/glm.hpp"

using vec2 = glm::vec<2, fixed16, glm::defaultp>;
using vec3 = glm::vec<3, fixed16, glm::defaultp>;
using vec4 = glm::vec<4, fixed16, glm::defaultp>;
using mat3 = glm::mat<3, 3, fixed16, glm::defaultp>;
using mat4 = glm::mat<4, 4, fixed16, glm::defaultp>;
using quat = glm::qua<fixed16, glm::defaultp>;

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