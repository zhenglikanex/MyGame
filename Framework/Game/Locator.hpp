#pragma once

#include <cassert>
#include <unordered_map>
#include <memory>

#include "Framework/Game/Service.hpp"

#include "entt/entt.hpp"

class Locator
{
public:
	using TypeGenerate = entt::family<Locator>;

	Locator() = default;
	~Locator()
	{
		int a = 10;
	}
	
	Locator(const Locator&) = delete;
	Locator(Locator&&) = default;
	Locator& operator=(const Locator&) = delete;
	Locator& operator=(Locator&&) = default;

	template<class Type>
	void Set(std::unique_ptr<Type>&& service)
	{
		auto id = TypeGenerate::type<Type>;
		services_.emplace(id,std::move(service));
	}

	template<class Type>
	void Reset()
	{
		auto id = TypeGenerate::type<std::remove_const_t<Type>>;
		services_.erase(id);
	}

	template<class Type>
	Type& Ref()
	{
		auto id = TypeGenerate::type<std::remove_const_t<Type>>;

		assert(services_.find(id) != services_.end() && "not find service");

		return dynamic_cast<Type&>(*services_[id]);
	}
private:
	std::unordered_map<uint32_t, std::unique_ptr<Service>> services_;
};