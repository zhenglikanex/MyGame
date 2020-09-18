#include "Timer.h"

namespace network 
{
	Timer::Timer(boost::asio::io_context& io_context, float millisec, const std::function<void()> handler)
		:millisec_(millisec), timer_(io_context,std::chrono::milliseconds(millisec_)),strand_(io_context),handler_(handler)
	{
		timer_.async_wait(strand_.wrap(std::bind(&Timer::Handler, this, std::placeholders::_1)));
	}

	Timer::~Timer()
	{
		timer_.cancel();
	}

	void Timer::Handler(const boost::system::error_code& error)
	{
		if (handler_)
		{
			handler_();
		}

		timer_.expires_at(timer_.expires_at() + std::chrono::milliseconds(millisec_));
		timer_.async_wait(strand_.wrap(std::bind(&Timer::Handler, this, std::placeholders::_1)));
	}
}