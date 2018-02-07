#ifndef CORE_LOG_HPP
#define CORE_LOG_HPP

namespace util
{
    enum level
    {
        k_error = 0,
        k_generic,
        k_verbose
    };
    
    void setVerbose();
    
    void message(const level p_level, const char* const p_message);
}

#endif
