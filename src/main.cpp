#include <exception>

#include <signal.h>

#include "core/engine.hpp"
#include "util/log.hpp"

namespace
{
    void handleSIGINT(int sig)
    {
        core::Engine::shutdown();
    }
}

int main(int argc, const char** argv)
{   
    //setup the signal handler
    struct sigaction action;
    action.sa_handler = handleSIGINT;
    sigaction(SIGINT, &action, 0);

    try
    {
        core::Engine::get().configure(argc, argv);
        core::Engine::get().run();
    }
    catch (const std::exception& e)
    {
        util::message(util::k_error, e.what());
        return 1;
    }
    
    return 0;
}
