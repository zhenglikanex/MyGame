#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Game/Service.hpp"

#include "entt/entt.hpp"

class Locator
{
public:
	using TypeGenerate = entt::family<Locator>;

	Locator() = default;
	~Locator() = default;
	
	Locator(const Locator&) = delete;
	Locator(Locator&&) = default;
	Locator& operator=(const Locator&) = delete;
	Locator& operator=(Locator&&) = default;

	template<class Type>
	void Set(std::shared_ptr<Type> service)
	{
		auto id = TypeGenerate::type<Type>;
		services_.emplace(id,service);
	}

	template<class Type>
	void Reset()
	{
		auto id = TypeGenerate::type<Type>;
		services_.erase(id);
	}

	template<class Type>
	std::shared_ptr<Type> Get()
	{
		auto id = TypeGenerate::type<Type>;
		auto iter = services_.find(id);
		if (iter != services_.end())
		{
			return std::dynamic_pointer_cast<Type>(iter->second);
		}
		return nullptr;
	}
	
	template<class Type>
	Type& Ref()
	{
		
		auto id = TypeGenerate::type<Type>;
		return *std::dynamic_pointer_cast<Type>(services_[id]);
	}
private:
	std::unordered_map<uint32_t, std::shared_ptr<Service>> services_;
};