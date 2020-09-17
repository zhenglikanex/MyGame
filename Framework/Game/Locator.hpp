#pragma once

#include <unordered_map>

#include "Framework/Game/Service.hpp"

#include "entt/entt.hpp"

class Locator
{
public:
	Locator() = default;
	~Locator() = default;
	
	Locator(const Locator&) = delete;
	Locator(Locator&&) = default;
	Locator& operator=(const Locator&) = delete;
	Locator& operator=(Locator&&) = default;

	template<class Type>
	void Set(std::unique_ptr<Type>&& service)
	{
		auto id = entt::family<Type>::type;
		services_.emplace(id, std::move(service));
	}

	template<class Type>
	void Reset()
	{
		auto id = entt::family<Type>::type;
		services_.erase(id);
	}

	template<class Type>
	std::unique_ptr<Type>& Get()
	{
		auto id = entt::family<Type>::type;
		auto iter = services_.find(id);
		if (iter != services_.end())
		{
			return std::dynamic_pointer_cast<Type>(*iter);
		}
		return nullptr;
	}
	
	template<class Type>
	Type& Ref()
	{
		auto id = entt::family<Type>::type;
		return *(services_[id]);
	}
private:
	std::unordered_map<uint32_t, std::unique_ptr<Service>> services_;
};