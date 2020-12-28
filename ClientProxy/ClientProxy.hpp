#pragma once

#include <cstdint>

#ifndef PROXY_DLL
#define PROXY_DLL __declspec(dllexport)
#endif

using Delegate = const char*(*)(const char*,const char*);

extern "C"
{
	PROXY_DLL void SetUnityDelegate(Delegate delegate);

	PROXY_DLL void InitGame(const char* data,int32_t size);

	PROXY_DLL void DestoryGame();
	PROXY_DLL void UpdateGame(float dt);
}