#include "engine.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include "core/configsetup.hpp"
#include "core/replay/replay.hpp"
#include "util/log.hpp"

namespace
{
    std::unique_ptr<core::Engine> s_engine;

    void printStat(const core::replay::Replay& p_playback)
    {
        std::stringstream ss;
        ss << "Rate = " << p_playback.getRate() << " Mbps, Entropy = " << p_playback.getFuzz() << " bpm" << std::flush;
        util::message(util::k_verbose, ss.str().c_str());
    }
}

core::Engine& core::Engine::get()
{
    if (s_engine.get() == nullptr)
    {
        s_engine.reset(new core::Engine());
    }

    return *s_engine;
}

void core::Engine::shutdown()
{
    if (core::Engine::get().m_shutdown == false)
    {
        util::message(util::k_generic, "Shutdown..");
        core::Engine::get().m_shutdown = true;
    }
}

bool core::Engine::isShuttingDown()
{
    return core::Engine::get().m_shutdown.load();
}

core::Engine::Engine() :
    m_playback(),
    m_config(),
    m_shutdown(false)
{
}

core::Engine::~Engine()
{
}

void core::Engine::configure(const int p_argc, const char** p_argv)
{
    core::configInit(m_config, p_argc, p_argv);

    m_playback.configure(m_config);
}

void core::Engine::info()
{
    while (isShuttingDown() == false)
    {
        //print the current playback rate if verbose
        if (m_config.m_verbose)
        {
            printStat(m_playback);
        }

        //sleep for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void core::Engine::run()
{
    //reporting thread
    std::thread reporting(&Engine::info, this);

    do
    {
        m_playback.go();
    }
    while (isShuttingDown() == false && m_config.m_loop);

    shutdown();
    reporting.join();

    printStat(m_playback);
}
