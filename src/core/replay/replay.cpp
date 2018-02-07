#include "replay.hpp"
#include "cap/packet.hpp"
#include "core/engine.hpp"
#include "core/config.hpp"
#include "output/device.hpp"
#include "util/log.hpp"
#include "util/fuzz.hpp"
#include "cap/file.hpp"
#include "core/config.hpp"
#include "output/device.hpp"
#include "util/fuzz.hpp"
#include "output/if/interface.hpp"
#include "output/tcp/socket.hpp"
#include "output/udp/socket.hpp"

#include <cassert>
#include <thread>


core::replay::Replay::Replay() :
    m_file(),
    m_fuzz(),
    m_output(),
    m_start(),
    m_bytesProcessed(0),
    m_rateConfig(0),
    m_rateCurrent(0),
    m_interval(0),
    m_loop(false)
{
}

core::replay::Replay::~Replay()
{
}

void core::replay::Replay::configure(const core::Config& p_config)
{
    //load the pcap file
    m_file.reset(new cap::File());
    m_file->load(p_config.m_filename.c_str());

    //configure the fuzzer
    m_fuzz.reset(new util::Fuzz());
    m_fuzz->configure(p_config.m_seed, p_config.m_entropy, p_config.m_protect);

    //configure the output device
    switch (p_config.m_outputType)
    {
        case k_odt_if:
            m_output.reset(new output::dev::Interface());
            break;
        case k_odt_tcp:
            m_output.reset(new output::tcp::Socket());
            break;
        case k_odt_udp:
            m_output.reset(new output::udp::Socket());
            break;
        default:
            break;
    }
    m_output->configure(p_config.m_socket.c_str(), p_config.m_port);

    //connect the output socket
    if (m_output->connect() == false)
    {
        util::message(util::k_generic, "Unable to connect to socket, waiting for available connection");

        while (::core::Engine::isShuttingDown() == false && m_output->connect() == false)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    util::message(util::k_generic, "Connected to socket");

    //verify the rate
    if (p_config.m_rate == 0)
    {
        util::message(util::k_generic, "Using default rate of 10Mbps");
    }

    const std::uint32_t rate = p_config.m_rate ? p_config.m_rate : 10;

    //get stuff from the configuration
    m_rateConfig = rate * 1024 * 128; //convert to Bps
    m_interval = rate; //estimate 1 packets per Mbps of rate
    m_loop = p_config.m_loop;
}

void core::replay::Replay::rateCheck()
{
    //get the current time and duration
    const std::chrono::high_resolution_clock::time_point cur = std::chrono::high_resolution_clock::now();
    const double duration = std::chrono::duration_cast<std::chrono::duration<double>>(cur - m_start).count();

    //calculate current rate in Bps
    m_rateCurrent = m_bytesProcessed / duration;

    const std::uint64_t expectedBytes = m_rateConfig * duration;


    //if we're going too fast, must sleep, at least 1ms
    if (m_bytesProcessed > expectedBytes)
    {
        const std::uint64_t msDelay = std::uint64_t((m_bytesProcessed - expectedBytes) / double(1000));

        //only sleep if delay is > 1ms
        if (msDelay)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(msDelay));
        }
    }
}

std::uint32_t core::replay::Replay::getFuzz() const
{
    return m_fuzz->getAppliedEntropy();
}

void core::replay::Replay::go()
{
    //reset all things in state not config related
    m_bytesProcessed = 0;
    m_rateCurrent = 0;
    m_start = std::chrono::high_resolution_clock::now();

    //local things need for replay adjustments
    std::uint32_t curInterval = 0;


    size_t i = 0;
    while (true)
    {
        //reference to current packet
        const cap::Packet& packet = m_fuzz->fuzz(m_file->getPacket(i));

        //ship the packet to the device
        m_output->writePacket(packet);

        //update the stats
        m_bytesProcessed += packet.m_size;

        //do we need a rate check?
        if (++curInterval == m_interval)
        {
            //check for a shutdown first
            if (core::Engine::isShuttingDown())
            {
                return;
            }

            //slowdown and reset check
            rateCheck();
            curInterval = 0;
        }

        if (++i == m_file->getPacketCount())
        {
            if (m_loop)
            {
                i = 0;
            }
            else
            {
                break;
            }
        }
    }

    //only really used to update m_rateCurrent
    rateCheck();

    const double duration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - m_start).count();
    const std::uint32_t Bps = m_bytesProcessed / duration;
    const std::uint32_t Mbps = (m_bytesProcessed / (1024 * 128)) / duration;
}
