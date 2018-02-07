#include "socket.hpp"
#include "cap/packet.hpp"

output::udp::Socket::Socket() :
    output::Device()
{
}

output::udp::Socket::~Socket()
{
}

void output::udp::Socket::configure()
{
}

bool output::udp::Socket::doConnect()
{
    return false;
}

void output::udp::Socket::doClose()
{
}

void output::udp::Socket::doWritePacket(const cap::Packet& /*p_packet*/)
{
}

void output::udp::Socket::doConfigure(const char* /*p_address*/, const std::uint16_t /*p_port*/)
{
}
