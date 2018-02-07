#ifndef CORE_REPLAY_REPLAY_HPP
#define CORE_REPLAY_REPLAY_HPP

#include <cstdint>
#include <chrono>
#include <memory>

//forwards
namespace core { class Config; }
namespace cap { class File; }
namespace output { class Device; }
namespace util { class Fuzz; }

namespace core
{
    namespace replay
    {
        class Replay
        {
        public:
            //! Constructor
            Replay();

            //! Destructor
            ~Replay();

            /*!
                Configure this object to replay data at the desired rate and
                integrity to the provided device.

                \param[in] p_config The configuration object to use
            */
            void configure(const core::Config& p_config);

            /*!
                Execute playback
            */
            void go();

            /*!
                \return The current rate in Mbps
            */
            std::uint32_t getRate() const
            {
                return m_rateCurrent / (1024 * 128);
            }

            /*!
                \return The current fuzz in ppm
            */
            std::uint32_t getFuzz() const;

        private:

            /*!
                Rate check!
            */
            void rateCheck();

        private:

            //! pcap file to run
            std::unique_ptr<cap::File> m_file;

            //! Fuzzer to use
            std::unique_ptr<util::Fuzz> m_fuzz;

            //! the output device which we will write to
            std::unique_ptr<output::Device> m_output;

            //! Time this replay started
            std::chrono::high_resolution_clock::time_point m_start;

            //! Number of bytes processed so far
            std::uint64_t m_bytesProcessed;

            //! Desired rate in Bps
            std::uint64_t m_rateConfig;

            //! Current rate in Bps
            std::uint64_t m_rateCurrent;

            //! Frequency to check our playback rate and shutdown status
            std::uint32_t m_interval;

            //! Loop playback?
            bool m_loop;
        };
    }
}

#endif
