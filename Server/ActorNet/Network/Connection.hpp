#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <cstdint>

#include "asio.hpp"

static const uint16_t kMaxMsgSize = (1 << 16) - 1;
static const uint8_t kHeadSize = 2;

using Buffer = std::vector<uint8_t>;

class Connection
{
public:
	Connection(uint32_t owner_id, asio::ip::tcp::socket&& socket);
	virtual ~Connection() = 0;

	void Receive();
	void Send(Buffer&& buffer);
	void Close();

	void set_receive_handler(const ReceiveHandler& handler) { receive_handler_ = handler; }
	void set_close_handler(const CloseHandler& handler) { close_handler_ = handler; }

	void set_id(uint16_t id) { id_ = id; }

	uint32_t id() const { return id_; }
	uint32_t owner_id() const { return owner_id_; }
private:
	void ReadHead();
	void ReadBody(uint16_t size);

	uint16_t id_;
	uint32_t owner_id_;
	asio::ip::tcp::socket socket_;
	std::array<uint8_t, kMaxMsgSize> data_;
	ReceiveHandler receive_handler_;
	CloseHandler close_handler_;
};

using ConnectHandler = std::function<void(std::unique_ptr<Connection>)>;
using ReceiveHandler = std::function<void(std::unique_ptr<Connection>, Buffer&&)>;
using SendCallback = std::function<void(std::unique_ptr<Connection>)>;
using CloseHandler = std::function<void(std::unique_ptr<Connection>)>;


#endif