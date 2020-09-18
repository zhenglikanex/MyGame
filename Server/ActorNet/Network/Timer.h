#pragma once

#include <functional>
#include <chrono>

#include "boost/asio/deadline_timer.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio.hpp"


namespace network
{
	class Timer
	{
	public:
		Timer(boost::asio::io_context& io_context, float millisec, const std::function<void()> handler);
		~Timer();
	private:
		void Handler(const boost::system::error_code& error);

		int millisec_;
		boost::asio::high_resolution_timer timer_;
		boost::asio::io_context::strand strand_;
		std::function<void()> handler_;
	};
}