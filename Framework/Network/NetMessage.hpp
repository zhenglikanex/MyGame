#pragma once

#include <vector>
#include <cstdint>
#include <string>

#include "3rdparty/include/kcp/ikcp.h"

struct IKCPCB;
typedef struct IKCPCB ikcpcb;

// indicate a converse between a client and connection_obj between server.
typedef uint32_t kcp_conv_t;

class NetMessage
{
public:
	void Parse(const uint8_t* data, size_t size)
	{
		std::copy_n(data, sizeof(uint16_t), &session_);
		data += sizeof(uint16_t);

		uint8_t name_size;
		std::copy_n(data, sizeof(uint8_t), &name_size);
		data += sizeof(uint8_t);

		name_ = std::string((char*)data, name_size);
		data += name_size;

		size = size - sizeof(uint16_t) - sizeof(uint8_t) - name_size;
		data_.resize(size);
		
		std::copy_n(data,size, data_.data());
	}

	void Serialize(uint8_t* out)
	{
		uint8_t name_size = name_.size();
		std::copy_n(&name_size, sizeof(uint8_t), out);
		out += sizeof(uint8_t);

		std::copy_n(name_.data(),name_.size(),out);
		out += name_.size();

		std::copy_n(data_.data(), data_.size(), out);
	}

	size_t GetByteSize() const
	{
		return sizeof(uint16_t) + sizeof(uint8_t) + name_.size() + data_.size();
	}

	void set_session(uint16_t session) { session_ = session; }

	void set_name(std::string_view name) 
	{ 
		assert(name.size() <= 255 && "超过最大名字限制");

		name_ = name;
	}
	void set_data(std::vector<uint8_t> data) { data_ = std::move(data); }

	uint16_t session() const { return session_; }
	const std::string& name() const { return name_; }
	const std::vector<uint8_t>& data() const { return data_; }
private:
	uint16_t session_;
	std::string name_;
	std::vector<uint8_t> data_;
};

const std::string kReqKcpConnect = "hello kcp world";
const std::string kRspKcpConnect = "welcome come to kcp:";
const std::string kKcpDisconnect = "out kcp";

inline std::string MakeReqKcpConnectMsg()
{
	return kReqKcpConnect;
}

inline std::string MakeRspKcpConnectMsg(kcp_conv_t conv)
{
	return kRspKcpConnect + std::to_string(conv);
}

inline std::string MakeKcpDisconnectMsg()
{
	return kKcpDisconnect;
}

inline bool IsReqKcpConnectMsg(const char* data,size_t size)
{
	return size == kReqKcpConnect.size() && memcmp(kReqKcpConnect.data(), data, size) == 0;
}

inline bool IsRspKcpConnectMsg(const char* data,size_t size)
{
	return size > kRspKcpConnect.size() && memcmp(data, kRspKcpConnect.data(), kRspKcpConnect.size()) == 0;
}

inline bool IsDisconnectMsg(const std::string& msg)
{
	return msg == kKcpDisconnect;
}

inline kcp_conv_t GetKcpConv(const char* data)
{
	return std::atol(data + kRspKcpConnect.size());
}

class KcpMessage
{
public:
	enum class KcpMessageType : uint8_t
	{
		kTypeConnected,
		kTypeDisconnect,
		kTypeRpc,
		kTypeRequest,
		kTypeMax,
	};

	KcpMessage()
		: type_(KcpMessageType::kTypeMax)
	{

	}

	void Parse(const uint8_t* data, size_t size)
	{
		uint8_t type = static_cast<uint8_t>(type_);
		std::copy_n(data, sizeof(uint8_t),&type);
		data += sizeof(uint8_t);
		
		if (size == 0)
		{
			return;
		}

		net_message_.Parse(data, size - sizeof(uint8_t));
	}

	void Serialize(uint8_t* out)
	{
		uint8_t type = static_cast<uint8_t>(type_);
		std::copy_n(&type, sizeof(uint8_t), out);
		out += sizeof(uint8_t);
		
		net_message_.Serialize(out);
	}

	size_t GetByteSize() const
	{
		return sizeof(uint8_t) + net_message_.GetByteSize();
	}
	
	NetMessage MoveNetMessage() { return std::move(net_message_); }

	void set_type(KcpMessageType type) { type_ = type; }
	void set_net_message(NetMessage net_message) { net_message_ = std::move(net_message_); }

	KcpMessageType type() const { return type_; }
private:
	KcpMessageType type_;
	NetMessage net_message_;
};
