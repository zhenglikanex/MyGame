#pragma once

#include <string_view>
#include <queue>

#include "nlohmann/json.hpp"

#ifdef EXPORT
#define EXPORT_DLL __declspec(dllexport)
#else
#define EXPORT_DLL __declspec(dllimport)
#endif

// param1:函数名
// param2:json格式参数
using UnityDelegate = const char*(*)(const char*,const char*);

template<class T> struct TypeSig {  };
template<> struct TypeSig<void> : std::integral_constant<char, 'V'> {};
template<> struct TypeSig<int32_t> : std::integral_constant<char, 'I'> {};
template<> struct TypeSig<uint32_t> : std::integral_constant<uint32_t, 'U'> {};
template<> struct TypeSig<int64_t> : std::integral_constant<char, 'L'> {};
template<> struct TypeSig<float> : std::integral_constant<char, 'F'> {};
template<> struct TypeSig<double> : std::integral_constant<char, 'D'> {};
template<> struct TypeSig<std::string> : std::integral_constant<char, 'S'> {};
template<> struct TypeSig<const char*> : std::integral_constant<char, 'S'> {};
template<> struct TypeSig<char*> : std::integral_constant<char, 'S'> {};

class UnityBridge
{
public:
	static UnityBridge& Get()
	{
		if (!instance_)
		{
			instance_ = std::make_unique<UnityBridge>();
		}

		return *instance_;
	}

	static void SetExecuteDelegate(UnityDelegate delegate)
	{ 
		unity_delegate_ = delegate; 
	}

	template<class R, class ... Args>
	R CallUnity(std::string_view fun_name, Args&& ... args)
	{
		//char fun_sig[sizeof...(args) + 2] = { TypeSig<std::remove_cv_t<R>>::value,TypeSig<std::remove_cv_t<std::decay_t<Args>>>::value...,0 };
		nlohmann::json json;
		(json.push_back(args), ...);

		std::string params = json.dump();
		if constexpr (!std::is_void_v<R>)
		{
			std::string str = unity_delegate_(fun_name.data(),params.c_str());
			nlohmann::json json = nlohmann::json::parse(str);
			return json["result"].get<R>();
		}
		else
		{
			unity_delegate_(fun_name.data(),params.c_str());
		}
	}
private:
	static UnityDelegate unity_delegate_;
	static std::unique_ptr<UnityBridge> instance_;
};

extern "C"
{
	EXPORT_DLL void SetUnityDelegate(UnityDelegate delegate);
}
