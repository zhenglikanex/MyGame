#include "ClientProxy.hpp"

#include <cstdint>
#include <string>
#include <Windows.h>

extern "C"
{
	HMODULE  instance = 0;

	PROXY_DLL void LoadDLL(const char* path)
	{
		TCHAR chCurDir[MAX_PATH] = { 0 };
		GetCurrentDirectory(MAX_PATH, chCurDir);
		SetCurrentDirectory(path);

		std::string name = std::string(path) + "MyGameClient.dll";
		instance = LoadLibrary(name.c_str());

		SetCurrentDirectory(chCurDir);

		if(!instance)
		{
			auto error = GetLastError();
			(void)error;
			int a = 19;
		}
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