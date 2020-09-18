#include <thread>
#include <chrono>

#include <asio.hpp>

#include "Framework/Game/Game.hpp"
#include "Framework/Game/NetworkService.hpp"

using namespace asio;

class UnityNetworkService : public NetworkService
{
	void Send(uint8_t* data, uint32_t len) override
	{

	}
};

int main()
{
	io_context context;

	Locator locator;
	locator.Set<NetworkService>(std::make_shared<UnityNetworkService>());
	Game game(std::move(locator), GameMode::kClinet);
	float dt = 0;
	while (true)
	{
		game.Update(dt);
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
	return 0;
}