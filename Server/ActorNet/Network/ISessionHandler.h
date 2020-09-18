#pragma once

#include "Message.h"

namespace network {
	class Session;

	// 会话处理接口
	class ISessionHandlder
	{
	public:
		virtual ~ISessionHandlder() = 0 {}

		virtual void SessionConnectHandler(const std::shared_ptr<network::Session>& session_ptr)
		{

		}

		virtual void SessionReceiveHandler(const std::shared_ptr<network::Session>& session_ptr, const network::Message& message) = 0;

		virtual void SessionCloseHander(const std::shared_ptr<network::Session>& session_ptr)
		{

		}
	};
}