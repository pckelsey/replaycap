#ifndef OUTPUT_DEVICE_HPP
#define OUTPUT_DEVICE_HPP

#include <cstdint>
#include <sstream>

#include "cap/packet.hpp"
#include "util/log.hpp"

namespace output
{
    class Device
    {
    protected:
        /*!
            Only the derived may construct this class
        */
        Device() :
            m_countPacket(0),
            m_countBytes(0)
        {
        }

    public:
        /*!
            Destructor, does nothing
        */
        ~Device()
        {
        }

    public:
        /*!
            Write a packet to this output device

            \param[in] p_packet The packet to write
        */
        void writePacket(const cap::Packet& p_packet)
        {
            ++m_countPacket;
            m_countBytes += p_packet.m_size;
            doWritePacket(p_packet);
        }

        /*!
            Shutdown this output device
        */
        void close()
        {
            doClose();
        }

        /*!
            Ask the device to connect
        */
        bool connect()
        {
            return doConnect();
        }

        /*!
            Configure the device
        */
        void configure(const char* p_address, const std::uint16_t p_port)
        {
            std::stringstream msg;
            msg << "Output device configuration - " << p_address << ":" << p_port << std::flush;
            util::message(util::k_verbose, msg.str().c_str());

            doConfigure(p_address, p_port);
        }

    private:
        //! Must be implemented by the derived
        virtual void doWritePacket(const cap::Packet& p_packet) = 0;

        //! Must be implemented by the derived
        virtual void doClose() = 0;

        //! Configure the device connect
        virtual void doConfigure(const char* p_address, const std::uint16_t p_port) = 0;

        //! Have the device connect
        virtual bool doConnect() = 0;

    private:
        //! Counter for the number of packets written out to this device
        std::uint64_t m_countPacket;

        //! Counter for the number of bytes written out to this device
        std::uint64_t m_countBytes;
    };
}

#endif
