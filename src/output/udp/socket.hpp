#ifndef OUTPUT_UDP_SOCKET_HPP
#define OUTPUT_UDP_SOCKET_HPP

#include "output/device.hpp"

namespace cap { class Packet; }

namespace output
{
    namespace udp
    {
        class Socket : public ::output::Device
        {
        public:
            //! Constructor
            Socket();

            //! Destructor
            ~Socket();

            /*!
                Configure the socket connection.
            */
            void configure();

        private:
            //! Write the packet
            virtual void doWritePacket(const cap::Packet& p_packet);

            //! Close the socket connection
            virtual void doClose();

            //! Configure the socket connection.
            virtual void doConfigure(const char* p_address, const std::uint16_t p_port);

            //! Connect the already configured socket
            virtual bool doConnect();
        };
    }
}

#endif
