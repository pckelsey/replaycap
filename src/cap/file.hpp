#ifndef __FILE_HPP
#define __FILE_HPP

#include <memory>
#include <vector>
#include <cstdint>

class pcap_pkthdr;

namespace cap
{
    class Packet;
    class File
    {
    public:
        //! Constructor
        File();
        
        //! Destructor
        ~File();
        
        //! \param[in] p_filename The file to load
        void load(const char* const p_filename);
        
        //! \return the number of packets in this file
        size_t getPacketCount() const;
        
        //! \return a reference to the packet
        const Packet& getPacket(const size_t p_index) const;
        
    private:
        //! append a packet
        void appendPacket(const pcap_pkthdr* p_header, const std::uint8_t* p_data);
        
    private:
        File(const File&);
        
        File& operator=(const File&);
    
    private:
        //buffer to store pcap data
        std::vector<std::uint8_t*> m_file;
        
        //vector of packets only
        std::vector<Packet> m_packet;
        
        //bytes into the current buffer
        size_t m_fileOffset;
    };
}

#endif
