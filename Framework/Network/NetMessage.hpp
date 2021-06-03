#pragma once

#include <vector>
#include <cstdint>
#include <string>

#include "3rdparty/include/kcp/ikcp.h"
#include "3rdparty/include/google/protobuf/message.h"

struct IKCPCB;
typedef struct IKCPCB ikcpcb;

// indicate a converse between a client and connection_obj between server.
typedef uint32_t kcp_conv_t;

class NetMessage
{
public:
	NetMessage()
		: session_(0)
	{

	}

	NetMessage(NetMessage&& rhs) noexcept
		: session_(rhs.session_)
		, name_(std::move(rhs.name_))
		, data_(std::move(rhs.data_))
	{

	}

	NetMessage& operator=(NetMessage&& rhs) noexcept
	{
		if (this == &rhs)
		{
			return *this;
		}

		session_ = rhs.session_;
		name_ = std::move(rhs.name_);
		data_ = std::move(rhs.data_);

		return *this;
	}

	NetMessage(const NetMessage&) = default;
	NetMessage& operator=(const NetMessage&) = default;

	void Parse(const uint8_t* data, size_t size)
	{
		std::copy_n(data, sizeof(uint16_t), (uint8_t*)&session_);
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
		std::copy_n((uint8_t*)&session_, sizeof(uint16_t), out);
		out += sizeof(uint16_t);
		
		uint8_t name_size = name_.size();
		std::copy_n((uint8_t*)&name_size, sizeof(uint8_t), out);
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

const std::string kReqKcpConnect = "Hello kcp world";
const std::string kRspKcpConnect = "Welcome come to kcp:";
const std::string kKcpConnected = "I have joined the world of kcp";
const std::string kKcpDisconnected = "Out kcp";

inline std::string MakeReqKcpConnectMsg()
{
	return kReqKcpConnect;
}

inline std::string MakeRspKcpConnectMsg(kcp_conv_t conv)
{
	return kRspKcpConnect + std::to_string(conv);
}

inline std::string MakeKcpConnectedMsg()
{
	return kKcpConnected;
}

inline std::string MakeKcpDisconnectMsg()
{
	return kKcpDisconnected;
}

inline bool IsReqKcpConnectMsg(const char* data,size_t size)
{
	return size == kReqKcpConnect.size() && memcmp(kReqKcpConnect.data(), data, size) == 0;
}

inline bool IsRspKcpConnectMsg(const char* data,size_t size)
{
	return size > kRspKcpConnect.size() && memcmp(data, kRspKcpConnect.data(), kRspKcpConnect.size()) == 0;
}

inline bool IsKcpConnectedMsg(const char* data, size_t size)
{
	return size == kKcpConnected.size() && memcmp(data, kKcpConnected.data(), kKcpConnected.size()) == 0;
}

inline bool IsKcpDisconnectMsg(const char* data,size_t size)
{
	return size == kKcpDisconnected.size() && memcmp(data, kKcpDisconnected.data(), kKcpDisconnected.size()) == 0;
}

inline kcp_conv_t GetKcpConv(const char* data)
{
	return std::atol(data + kRspKcpConnect.size());
}

template<class T>
std::enable_if_t<std::is_base_of_v<google::protobuf::Message, T>,std::vector<uint8_t>>
Serialize(const T& data)
{
	std::vector<uint8_t> buffer(data.ByteSize());
	data.SerializeToArray(buffer.data(), buffer.size());

	return buffer;
}