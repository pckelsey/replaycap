#include "socket.hpp"

#include "cap/packet.hpp"

#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

#include "util/log.hpp"

output::tcp::Socket::Socket() :
    output::Device(),
    m_server(),
    m_sock(-1)
{
}

output::tcp::Socket::~Socket()
{
}

void output::tcp::Socket::doConfigure(const char* p_addr, const std::uint16_t p_port)
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sock == -1)
    {
        //create the socket
        util::message(util::k_error, "unable to open socket connection");
        return;
    }

    //setup the socket address
    if (inet_addr(p_addr) == -1)
    {
        //nameresolution
        struct hostent *he = gethostbyname(p_addr);
        struct in_addr **addr_list;

        if (he == NULL)
        {
            util::message(util::k_error, "Unable to resolve hostname");
            return;
        }

        addr_list = (struct in_addr**) he->h_addr_list;
        for (int i = 0; addr_list[i] != NULL; ++i)
        {
            m_server.sin_addr = *addr_list[i];
        }
    }
    else
    {
        m_server.sin_addr.s_addr = inet_addr(p_addr);
    }

    //setup the port and socket options
    m_server.sin_family = AF_INET;
    m_server.sin_port = ntohs(p_port);
}

bool output::tcp::Socket::doConnect()
{
    if (::connect(m_sock, (struct sockaddr*)&m_server, sizeof(m_server)) < 0)
    {
        return false;
    }
    return true;
}

void output::tcp::Socket::doClose()
{
}

void output::tcp::Socket::doWritePacket(const cap::Packet& p_packet)
{
    if (send(m_sock, p_packet.m_data, p_packet.m_size, 0) < 0)
    {
        util::message(util::k_error, "Error sending data to socket");
    }
}
