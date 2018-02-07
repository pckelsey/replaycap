#ifndef __PACKET_HPP
#define __PACKET_HPP

#include <cstdint>

namespace cap
{
    const std::uint32_t s_maxFrameSize = 4096;

    struct Packet
    {
        std::uint32_t m_sec;
        std::uint32_t m_usec;
        std::uint32_t m_reserved;
        std::uint32_t m_size;
        std::uint8_t* m_data;
    };
}

#endif
