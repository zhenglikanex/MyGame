#include <cstdint>

enum class ProtoId : uint16_t
{
	CS_KcpConnection,
	SC_KcpConnection,
	CS_KcpDisconnection,
	SC_KcpDisconnection,
};