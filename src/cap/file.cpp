#include "file.hpp"
#include "packet.hpp"
#include "util/log.hpp"

#include <pcap.h>

#include <cstring>
#include <functional>
#include <sstream>

namespace cap
{
    const size_t s_allocSize = 1024 * 1024 * 10;

    void pcap_handle_packet(std::uint8_t* p_args, const pcap_pkthdr* p_header, const std::uint8_t* p_data)
    {
        if (p_header->caplen <= s_maxFrameSize)
        {
            //back into our member function
            (*reinterpret_cast<std::function<void(const pcap_pkthdr*,const std::uint8_t*)>*>(p_args))(p_header, p_data);
        }
    }
}

cap::File::File() :
    m_file(),
    m_packet(),
    m_fileOffset(s_allocSize)
{
}

cap::File::~File()
{
    //free file memory
    for (size_t i = 0; i != m_file.size(); ++i)
    {
        if (m_file[i] != NULL)
        {
            delete[] m_file[i];
            m_file[i] = NULL;
        }
    }
}

size_t cap::File::getPacketCount() const
{
    return m_packet.size();
}

const cap::Packet& cap::File::getPacket(const size_t p_index) const
{
    return m_packet.at(p_index);
}

void cap::File::appendPacket(const pcap_pkthdr* p_header, const std::uint8_t* p_data)
{
    m_packet.emplace_back();
    Packet& curpacket = m_packet.back();

    curpacket.m_sec = p_header->ts.tv_sec;
    curpacket.m_usec = p_header->ts.tv_usec;
    curpacket.m_size = p_header->caplen;

    //see if we need to alloc a new bucket
    if (m_fileOffset + curpacket.m_size > s_allocSize)
    {
        m_file.push_back(new std::uint8_t[s_allocSize]);
        m_fileOffset = 0;
    }

    //use back of file
    curpacket.m_data = m_file.back() + m_fileOffset;
    m_fileOffset += curpacket.m_size;

    memcpy(curpacket.m_data, p_data, curpacket.m_size);
}

void cap::File::load(const char *const p_filename)
{
    //log message for opening the file
    {
        std::stringstream msg;
        msg << "Opening pcap - " << p_filename << std::flush;
        util::message(util::k_verbose, msg.str().c_str());
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    size_t curpos = 0;
    pcap_t* descr = NULL;

    //attempt to open the file
    descr = pcap_open_offline(p_filename, errbuf);
    if (descr == NULL)
    {
        throw std::runtime_error(errbuf);
    }

    //callback into this class for out c-style callback
    std::function<void(const pcap_pkthdr*,const std::uint8_t*)> packetCallback =
        std::bind(&File::appendPacket, std::ref(*this), std::placeholders::_1, std::placeholders::_2);

    pcap_loop(descr, 0, pcap_handle_packet, reinterpret_cast<std::uint8_t*>(&packetCallback));

    //log message for opening the file
    {
        std::stringstream msg;
        msg << "Finished loading pcap, " << m_packet.size() << " packets, " << m_file.size() * 10 << " MB" << std::flush;
        util::message(util::k_verbose, msg.str().c_str());
    }
}
