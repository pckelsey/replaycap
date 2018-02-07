#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include <cstdint>
#include <string>

enum OutputDeviceType
{
    k_odt_tcp,
    k_odt_udp,
    k_odt_if
};

namespace core
{
    struct Config
    {
        std::string m_filename;
        std::string m_socket;
        OutputDeviceType m_outputType;
        std::uint32_t m_entropy;
        std::uint32_t m_protect;
        std::uint32_t m_rate;
        std::uint32_t m_seed;
        std::uint16_t m_port;
        bool m_verbose;
        bool m_loop;
    };
}

#endif
