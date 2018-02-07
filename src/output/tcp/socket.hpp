#ifndef OUTPUT_TCP_SOCKET_HPP
#define OUTPUT_TCP_SOCKET_HPP

#include <arpa/inet.h>
#include "output/device.hpp"

// // class _socket_impl;

namespace output
{
    namespace tcp
    {
        class Socket : public ::output::Device
        {
        public:
            //! Constructor
            Socket();

            //! Destructor
            ~Socket();

        private:
            //! Write the packet
            virtual void doWritePacket(const cap::Packet& p_packet);

            //! Close the socket connection
            virtual void doClose();

            //! Configure the socket connection.
            virtual void doConfigure(const char* p_address, const std::uint16_t p_port);

            //! Connect the already configured socket
            virtual bool doConnect();

        private:
            //server endpoint, store here for reconnects without config
            sockaddr_in m_server;

            //socket fd
            int m_sock;
        };
    }
}

#endif
