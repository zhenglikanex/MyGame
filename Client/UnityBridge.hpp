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
		if constexpr (sizeof...(Args) > 0)
		{
			nlohmann::json json;
			(json.push_back(args), ...);
			
			std::string params = json.dump();

			if constexpr (!std::is_void_v<R>)
			{
				std::string str = unity_delegate_(fun_name.data(), params.c_str());
				nlohmann::json json = nlohmann::json::parse(str);
				return json["result"].get<R>();
			}
			else
			{
				unity_delegate_(fun_name.data(), params.c_str());
			}
		}
		else
		{
			if constexpr (!std::is_void_v<R>)
			{
				std::string str = unity_delegate_(fun_name.data(),"");
				nlohmann::json json = nlohmann::json::parse(str);
				return json["result"].get<R>();
			}
			else
			{
				unity_delegate_(fun_name.data(),"");
			}
		}
	}
private:
	static UnityDelegate unity_delegate_;
	static std::unique_ptr<UnityBridge> instance_;
};

extern "C"
{
	EXPORT_DLL void SetUnityDelegate(UnityDelegate delegate);
	EXPORT_DLL void GameRollback();
}
