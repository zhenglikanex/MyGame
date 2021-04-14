#ifndef TIMER_ACTOR_H_
#define TIMER_ACTOR_H_

#include <functional>
#include <mutex>
#include <chrono>

#include "asio.hpp"

namespace actor_net
{
	class ActorNet;

	class TimerActor;

	class Timer
	{
	public:
		Timer(TimerActor* timer_actor,asio::io_context& io_context, uint32_t owner, uint32_t millisec, int32_t repeat, const std::function<void()>& callback);
		~Timer();

		Timer(const Timer&) = delete;
		Timer(Timer&&) = delete;
		Timer& operator=(const Timer&) = delete;
		Timer& operator=(Timer&&) = delete;

		void Start();
		bool IsDone() const;
		uint32_t owner() const { return owner_; }
	private:
		TimerActor* timer_actor_;
		asio::steady_timer timer_;
		uint32_t owner_;
		uint32_t millisec_;
		int32_t repeat_;
		std::function<void()> callback_;
	};	

	class TimerActor
	{
	public:
		TimerActor(const std::shared_ptr<ActorNet>& actor_net);
		~TimerActor();

		void Run();
		void Stop();

		uint32_t AddTimer(uint32_t owner, uint32_t millisec, int32_t repeat, const std::function<void()>& callback);
		void CancelTimer(uint32_t id);

		void Post(uint32_t owner, const std::function<void()>& callback);
	private:		
		std::shared_ptr<ActorNet> actor_net_;
		asio::io_context io_context_;
		asio::io_context::work work_;
		std::unordered_map<uint32_t,std::unique_ptr<Timer>> timer_map_;
		std::atomic<uint32_t> alloc_id_;
	};
}

#endif