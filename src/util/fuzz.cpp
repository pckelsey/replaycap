#include "fuzz.hpp"

#include <cstring>
#include <algorithm>

#include <nmmintrin.h>

namespace
{
    std::uint32_t nextRand(std::uint32_t& p_rand)
    {
        p_rand = _mm_crc32_u32(p_rand, 2531011);
        return (p_rand >> 3) % 1000000;
    }
}

util::Fuzz::Fuzz() :
    m_packet(),
    m_buffer(),
    m_bytesTotal(0),
    m_bytesFuzzed(0),
    m_fuzzableBytes(0),
    m_rand(0),
    m_entropy(0),
    m_protect(0)
{
}

util::Fuzz::~Fuzz()
{
}

void util::Fuzz::configure(const std::uint32_t p_seed, const std::uint32_t p_entropy, const std::uint32_t p_protect)
{
    //our packet will always point to our buffer
    m_packet.m_data = m_buffer.data();
    m_entropy = p_entropy;
    m_rand = p_seed;
    m_protect = p_protect;
}

const cap::Packet& util::Fuzz::fuzz(const cap::Packet& p_packet)
{
    //count the bytes
    m_bytesTotal += p_packet.m_size;

    //no-op?
    if (m_entropy == 0 || m_protect >= p_packet.m_size)
    {
        return p_packet;
    }
    else
    {
        memcpy(m_packet.m_data, p_packet.m_data, p_packet.m_size);
    }

    for (std::uint32_t i = m_protect; i != p_packet.m_size; ++i)
    {
        const std::uint32_t rand = nextRand(m_rand);
        if (rand < m_entropy)
        {
            m_packet.m_data[i] ^= m_rand;
            ++m_bytesFuzzed;
        }
    }

    //copy packet information
    m_packet.m_sec = p_packet.m_sec;
    m_packet.m_usec = p_packet.m_usec;
    m_packet.m_size = p_packet.m_size;

    m_fuzzableBytes += m_packet.m_size - m_protect;

    //return our packet
    return m_packet;
}

