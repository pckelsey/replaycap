#include "log.hpp"
#include "scoped_lock.hpp"

#include <array>
#include <atomic>
#include <ctime>
#include <iostream>
#include <mutex>

namespace
{
    //mutex guarding stdout
    std::mutex s_outputMutex;

    //for the time info, protected by mutex
    char s_tmbuf[64];
    
    //log messages
    const std::array<std::string, 3> s_levelMsg = {{"ERROR  ", "GENERIC", "VERBOSE"}};
    
    //verbosity
    std::atomic_bool s_verbose(false);
}

void util::setVerbose()
{
    s_verbose = true;
}

void util::message(const util::level p_level, const char *const p_message)
{
    if (!s_verbose && p_level == util::k_verbose)
    {
        return;
    }
    
    scoped_lock<std::mutex&>(std::ref(s_outputMutex));

    //get the time, store in buffer
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    
    const size_t endpos = strftime(s_tmbuf, sizeof(s_tmbuf), "%Y%m%d%H%M%S", timeinfo);
    s_tmbuf[endpos] = 0;
    
    //print the message
    std::cout << s_tmbuf << ": " << s_levelMsg[p_level] << ": " << p_message << std::endl;
}
