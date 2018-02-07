#ifndef OUTPUT_IF_INTERFACE_HPP
#define OUTPUT_IF_INTERFACE_HPP

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

#include "output/device.hpp"

namespace output
{
    namespace dev
    {
        class Interface : public ::output::Device
        {
        public:
            //! Constructor
            Interface();

            //! Destructor
            ~Interface();

        private:
            //! Write the packet
            virtual void doWritePacket(const cap::Packet& p_packet);

            //! Close the socket connection
            virtual void doClose();

            //! Configure the socket connection.
            virtual void doConfigure(const char* p_address, const std::uint16_t);

            //! Connect the already configured socket
            virtual bool doConnect();

        private:

            int m_sockfd;
            struct sockaddr_ll m_socket_address;
        };
    }
}

#endif
