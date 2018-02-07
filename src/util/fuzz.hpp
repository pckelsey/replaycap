#ifndef UTIL_FUZZ_HPP
#define UTIL_FUZZ_HPP

#include "cap/packet.hpp"

#include <array>

namespace util
{
    class Fuzz
    {
    public:
        //! Constructor
        Fuzz();

        //! Destructor
        ~Fuzz();

        //! Configure this guy with params
        void configure(const std::uint32_t p_seed, const std::uint32_t p_entropy, const std::uint32_t p_protect);

        /*!
            Fuzz a packet, returning a reference to the potentially modified packet
        */
        const cap::Packet& fuzz(const cap::Packet& p_packet);

        /*!
            Return the entropy applied, in ppm.
        */
        const std::uint32_t getAppliedEntropy()
        {

            return m_fuzzableBytes == 0 ?
                0 : std::uint32_t(double(m_bytesFuzzed * 1000000) / double(m_fuzzableBytes));
        }

    private:
        //! Packet which will be returned each time
        cap::Packet m_packet;

        //! Data buffer for our packet
        std::array<std::uint8_t, cap::s_maxFrameSize> m_buffer;

        //! Number of bytes total
        std::uint64_t m_bytesTotal;

        //! Number of bytes fuzzed
        std::uint64_t m_bytesFuzzed;

        //! Total number of fuzzable bytes
        std::uint64_t m_fuzzableBytes;

        //! Configured entropy
        std::uint32_t m_entropy;

        //! Configured seed
        std::uint32_t m_rand;

        //! Protection zone of packet
        std::uint32_t m_protect;
    };
}

#endif
