#include "ClientProxy.hpp"

#include <cstdint>
#include <Windows.h>

extern "C"
{
	HMODULE  instance = 0;

	PROXY_DLL void LoadDLL()
	{
		instance = LoadLibrary("MyGameClient.dll");
		
	}

	PROXY_DLL void FreeDLL()
	{
		if (instance)
		{
			DestoryGame();
			FreeLibrary(instance);
			instance = nullptr;
		}
	}

	PROXY_DLL void SetUnityDelegate(Delegate delegate)
	{
		auto fpFun = (void(*)(Delegate))GetProcAddress(instance,"SetUnityDelegate");
		if (fpFun)
		{
			fpFun(delegate);
		}
	}

	PROXY_DLL void InitGame(const char* data,int32_t size)
	{
		auto fpFun = (void(*)(const char*,int32_t))GetProcAddress(instance, "InitGame");
		if (fpFun)
		{
			fpFun(data,size);
		}
	}

	PROXY_DLL void DestoryGame()
	{
		auto fpFun = (void(*)())GetProcAddress(instance, "DestoryGame");
		if (fpFun)
		{
			fpFun();
		}
	}

	PROXY_DLL void UpdateGame(float dt)
	{
		auto fpFun = (void(*)(float))GetProcAddress(instance, "UpdateGame");
		if (fpFun)
		{
			fpFun(dt);
		}
	}

	PROXY_DLL void GameInput(const char* data, int32_t size)
	{
		auto fpFun = (void(*)(const char*, int32_t))GetProcAddress(instance, "GameInput");
		if (fpFun)
		{
			fpFun(data, size);
		}
	}
	
	PROXY_DLL void GameRollback()
	{
		auto fpFun = (void(*)())GetProcAddress(instance, "GameRollback");
		if (fpFun)
		{
			fpFun();
		}
	}

	PROXY_DLL void JoinMatch()
	{
		auto fpFun = (void(*)())GetProcAddress(instance, "JoinMatch");
		if (fpFun)
		{
			fpFun();
		}
	}
}