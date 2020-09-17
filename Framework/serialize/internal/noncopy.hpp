#pragma once

namespace serialize
{
	class Noncopy
	{
	public:
		Noncopy() = default;
		~Noncopy() = default;

		Noncopy(const Noncopy&) = delete;
		Noncopy& operator=(const Noncopy&) = delete;
	};
}