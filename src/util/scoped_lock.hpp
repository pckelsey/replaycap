#ifndef UTIL_SCOPED_LOCK_HPP
#define UTIL_SCOPED_LOCK_HPP

//basic scoped lock, i'm not waiting around for c++17..
namespace util
{
    template <typename t_mutex> 
    struct scoped_lock
    {
        explicit scoped_lock(t_mutex& p_mutex) :
            m_mutex(p_mutex)
        {
            m_mutex.lock();
        }
        
        ~scoped_lock()
        {
            m_mutex.unlock();
        }
        
    private:
        t_mutex& m_mutex;
    };
}
    
#endif
