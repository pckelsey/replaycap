#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include <atomic>
#include <memory>
#include "core/replay/replay.hpp"
#include "core/config.hpp"

namespace core
{
    class Engine
    {
    public:
        //! Use this to get a copy of the engine
        static Engine& get();

        //! Use this to shutdown the engine
        static void shutdown();

        //! Query the shutdown request status of the engine
        static bool isShuttingDown();

        //! Destructor
        ~Engine();

        //! Setup the engine with params
        void configure(const int p_argc, const char** p_argv);

        //! Start the engine
        void run();

    private:
        //! Constructor
        Engine();

        //! Report the status
        void info();

    private:
        //! Replay device
        core::replay::Replay m_playback;

        //! Config for this engine
        core::Config m_config;

        //! True if the engine must shutdown
        std::atomic_bool m_shutdown;
    };
}

#endif
