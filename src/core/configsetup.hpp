#ifndef CORE_CONFIGSETUP_HPP
#define CORE_CONFIGSETUP_HPP

#include <boost/program_options.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_difference.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/include/qi_kleene.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_parse_attr.hpp>
#include <boost/spirit/include/qi_sequence.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <boost/spirit/include/qi_attr.hpp>
#include <boost/spirit/include/qi_alternative.hpp>
#include "config.hpp"
#include "util/log.hpp"

namespace core
{
    void configInit(Config& p_config, const int p_argc, const char** p_argv)
    {
        std::string addrport;
        boost::program_options::options_description desc("Options");
        desc.add_options()
            ("help,h", "Print help messages")
            ("verbose,v", "verbose log output")
            ("input,i", boost::program_options::value<std::string>(&(p_config.m_filename))->required(), "pcap file to load")
            ("output,o", boost::program_options::value<std::string>(&addrport)->required(), "output [tcp:ip:port, udp:ip:port, if:interface]")
            ("entropy,e", boost::program_options::value<std::uint32_t>(&(p_config.m_entropy)), "entropy in ppm")
            ("protect,p", boost::program_options::value<std::uint32_t>(&(p_config.m_protect)), "protect this many bytes in the beginning of the payload")
            ("rate,r", boost::program_options::value<std::uint32_t>(&(p_config.m_rate)), "rate in Mbps")
            ("seed,s", boost::program_options::value<std::uint32_t>(&(p_config.m_seed)), "seed for applying entropy")
            ("loop,l", "loop playback");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(p_argc, p_argv, desc), vm);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return;
        }
        if (vm.count("verbose"))
        {
            printf("setting verbose\n");
            util::setVerbose();
            p_config.m_verbose = true;
        }
        if (vm.count("loop"))
        {
            p_config.m_loop = true;
        }

        boost::program_options::notify(vm);

        if (boost::spirit::qi::parse(
            addrport.begin(),
            addrport.end(),
                (
                    boost::spirit::qi::attr(k_odt_tcp) >>
                    boost::spirit::qi::lit("tcp:") >>
                    *(
                        boost::spirit::qi::char_ - boost::spirit::qi::lit(':')
                    )
                    >> boost::spirit::qi::lit(':')
                    >> boost::spirit::qi::uint_
                )
                |
                (
                    boost::spirit::qi::attr(k_odt_udp) >>
                    boost::spirit::qi::lit("udp:") >>
                    *(
                        boost::spirit::qi::char_ - boost::spirit::qi::lit(':')
                    )
                    >> boost::spirit::qi::lit(':')
                    >> boost::spirit::qi::uint_
                )
                |
                (
                    boost::spirit::qi::attr(k_odt_if) >>
                    boost::spirit::qi::lit("if:") >>
                    *(
                        boost::spirit::qi::char_ - boost::spirit::qi::lit(':')
                    )
                    >> boost::spirit::qi::attr(0)
                ),
                p_config.m_outputType,
                p_config.m_socket,
                p_config.m_port) == false)
        {
            throw std::runtime_error("Unable to process the host:port information");
        }
    }
}

#endif
