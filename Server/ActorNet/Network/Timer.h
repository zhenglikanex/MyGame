#pragma once

#include <functional>
#include <chrono>

#include "asio.hpp"

namespace network
{
	class Timer
	{
	public:
		Timer(asio::io_context& io_context, float millisec, const std::function<void()> handler);
		~Timer();
	private:
		void Handler(const asio::error_code& error);

		int millisec_;
		asio::system_timer timer_;
		asio::io_context::strand strand_;
		std::function<void()> handler_;
	};
}