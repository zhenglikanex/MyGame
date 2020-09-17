#include <thread>
#include <chrono>

#include "Framework/Game/Game.hpp"
#include "Framework/Game/NetworkService.hpp"

class ServerNetworkService : public NetworkService
{
	void Send(uint8_t* data, uint32_t len) override
	{

	}
};

int main()
{
	Locator locator;
	locator.Set<NetworkService>(std::make_shared<ServerNetworkService>());
	Game game(std::move(locator), GameMode::kServer);
	float dt = 0;
	while (true)
	{
		game.Update(dt);
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
	return 0;
}