#pragma once

#include <vector>
#include <unordered_map>

#include "nlohmann/json.hpp"

struct base_array_key {};

template<class T>
struct array_key : base_array_key 
{
	static_assert(std::is_integral_v<T>, "T is not Integral!");
};

template<class T>
struct is_arry_key
{
	static constexpr bool value = std::is_base_of<base_array_key, T>::value;
};

class BaseConfig{};

template<class ... Ts>
class Config : public BaseConfig
{
public:
	template<class Key, class Value>
	struct Container
	{
		using type = std::conditional_t<
			is_arry_key<Key>::value,
			std::vector<Value>,
			std::unordered_map<Key, Value>
		>;
	};

	template<class Key, class ... Ts>
	struct Map;

	template<class Key, class ... Ts>
	struct Map
	{
		static_assert(sizeof...(Ts) > 0, "!");

		using type = typename Container<Key, typename Map<Ts...>::type>::type;
		using value_type = typename Map<Ts...>::value_type;
	};

	template<class Key, class Value>
	struct Map<Key, Value>
	{
		using type = typename Container<Key, Value>::type;
		using value_type = Value;
	};

	template<class Container, bool test>
	struct MapContainer
	{
		using value_type = typename Container::mapped_type;
	};

	template<class Container>
	struct MapContainer<Container, false>
	{
		using value_type = typename Container::value_type;
	};

	template<typename Container, class T, class ... Ts>
	struct ValueType
	{
		using type = typename ValueType<typename MapContainer<Container, !std::is_same<std::vector<typename Container::value_type>, Container>::value>::value_type, Ts...>::type;
	};

	template<typename Container, class T>
	struct ValueType<Container, T>
	{
		using type = typename MapContainer<Container, !std::is_same<std::vector<typename Container::value_type>, Container>::value>::value_type;
	};

	using ConfigMap = typename Map<Ts...>::type;
	using Entry = typename Map<Ts...>::value_type;

	Config() = default;
	Config(Config&&) = default;
	Config& operator=(Config&&) = default;
	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;

	void Load(const std::string& data)
	{
		json j = json::parse(data);
		entries_ = j.get<ConfigMap>();
	}

	template<class ... Args>
	bool HasEntry(Args&& ... args) const
	{
		
	}

	template<class ... Args>
	const typename ValueType<ConfigMap, Args...>::type& GetEntry(Args&& ... args) const
	{
		using ReturnType = const typename ValueType<ConfigMap,Args...>::type&;
		return GetEntryImpl<ReturnType>(const_cast<Config*>(this)->entries_, std::forward<Args>(args)...);
	}
private:
	template<class ReturnType,class Container, class Key, class ... Args>
	ReturnType GetEntryImpl(Container& container, Key&& key, Args&& ... args) const
	{
		if constexpr (sizeof...(args) > 0)
		{
			return GetEntryImpl<ReturnType>(container[key],std::forward<Args>(args)...);
		}
		else
		{
			return container[key];
		}
	}

	template<class Container, class Key, class ... Args>
	bool HasEntryImpl(const Container& container, Key&& key, Args&& ... args) const
	{
		bool result = false;

		if constexpr (sizeof...(args) > 0)
		{
			auto iter = std::find(container.begin(), container.end(), key);
			if (iter != container.end())
			{
				return HasEntryImpl(container[key], std::forward<Args>(args)...);
			}

			return false;
		}
		else
		{
			return std::find(container.begin(), container.end(), key) != container.end();
		}
	}

	ConfigMap entries_;
};
