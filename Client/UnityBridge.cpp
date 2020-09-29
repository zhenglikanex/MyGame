#include "UnityBridge.hpp"

#include "Client/UnityViewService.hpp"


std::unique_ptr<UnityBridge> UnityBridge::instance_;
UnityDelegate UnityBridge::unity_delegate_ = nullptr;

extern "C"
{
	EXPROT_DLL void SetUnityDelegate(UnityDelegate delegate)
	{
		UnityBridge::Get().SetExecuteDelegate(delegate);
	}
}