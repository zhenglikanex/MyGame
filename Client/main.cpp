#include <thread>
#include <chrono>

#include <asio.hpp>

#include "Framework/Game/Game.hpp"
#include "Framework/Game/NetworkService.hpp"
#include "Framework/Game/ViewService.hpp"
#include "Framework/Game/IViewImpl.hpp"

#include "Framework/Proto/Battle.pb.h"

class UnityNetworkService : public NetworkService
{
	void Send(uint8_t* data, uint32_t len) override
	{

	}
};
