#include "TimerActor.hpp"

#include "ActorNet.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <chrono>

#ifdef _WIN32

#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#define TIME_BEGIN_PERIOD(ms) ::timeBeginPeriod(ms)
#define TIME_END_PERIOD(ms) ::timeEndPeriod(ms)

#else

#define TIME_BEGIN_PERIOD(ms) 
#define TIME_END_PERIOD(ms)

#endif // Windows

using namespace std::chrono;

namespace actor_net
{
	Timer::Timer(TimerActor* timer_actor,asio::io_context& io_context, uint32_t owner, uint32_t millisec, int32_t repeat, const std::function<void()>& callback)
		: timer_actor_(timer_actor)
		, timer_(io_context,steady_clock::now())
		, owner_(owner)
		, millisec_(millisec)
		, repeat_(repeat)
		, callback_(callback)
	{
		Start();
	}

	Timer::~Timer()
	{
		timer_actor_ = nullptr;
		timer_.cancel();
	}

	void Timer::Start()
	{
		timer_.expires_at(timer_.expires_at() + milliseconds(millisec_));
		timer_.async_wait([this](std::error_code ec) {
			if (!ec)
			{
				--repeat_;
				if (!IsDone())
				{
					Start();
				}

				if (timer_actor_)
				{
					timer_actor_->Post(owner_,callback_);
				}
			}
		});
	}

	bool Timer::IsDone() const
	{
		return repeat_ == 0;
	}

	TimerActor::TimerActor(const std::shared_ptr<ActorNet>& actor_net)
		: actor_net_(actor_net)
		, alloc_id_(0)
		, work_(io_context_)
	{

	}

	TimerActor::~TimerActor()
	{
		
	}

	void TimerActor::Run()
	{
		TIME_BEGIN_PERIOD(1);
		io_context_.run();
		TIME_END_PERIOD(1);
	}

	void TimerActor::Stop()
	{
		io_context_.stop();
	}

	uint32_t TimerActor::AddTimer(uint32_t owner, uint32_t millisec, int32_t repeat, const std::function<void()>& callback)
	{
		auto id = alloc_id_++;
		io_context_.post([id, owner, millisec, repeat, callback, this]()
		{
			timer_map_.emplace(id, std::make_unique<Timer>(this,io_context_,owner,millisec,repeat,callback));
		});
		return id;
	}

	void TimerActor::CancelTimer(uint32_t id)
	{
		io_context_.post([id,this]() {
			timer_map_.erase(id);
		});
	}

	void TimerActor::Post(uint32_t owner,const std::function<void()>& callback)
	{
		std::any data(callback);
		actor_net_->SendActorMessage(0, owner, 0, ActorMessage::MessageType::kTypeTimer, "", std::move(data));
	}

}