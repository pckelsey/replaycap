#include "interface.hpp"
#include "core/engine.hpp"
#include "cap/packet.hpp"

output::dev::Interface::Interface() :
    output::Device(),
    m_sockfd(-1),
    m_socket_address()

{
}

output::dev::Interface::~Interface()
{
}

bool output::dev::Interface::doConnect()
{
    return m_sockfd > -1;
}

void output::dev::Interface::doClose()
{
}

void output::dev::Interface::doWritePacket(const cap::Packet& p_packet)
{
    const int result = sendto(
        m_sockfd,
        p_packet.m_data,
        p_packet.m_size,
        0,
        (struct sockaddr*)&m_socket_address, sizeof(struct sockaddr_ll));

    if (result < 0)
    {
        util::message(util::k_error, "Error writing packet to interface");
        ::core::Engine::shutdown();
    }
}

void output::dev::Interface::doConfigure(const char* p_address, const std::uint16_t)
{
    struct ifreq if_idx;
    struct ifreq if_mac;

    if ((m_sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) != -1)
    {
        //get the index of the desired interface
        memset(&if_idx, 0, sizeof(struct ifreq));
        strncpy(if_idx.ifr_name, p_address, IFNAMSIZ - 1);
        if (ioctl(m_sockfd, SIOCGIFINDEX, &if_idx) >= 0)
        {

            //get the mac address of the desired interface
            memset(&if_mac, 0, sizeof(struct ifreq));
            strncpy(if_mac.ifr_name, p_address, IFNAMSIZ - 1);
            if (ioctl(m_sockfd, SIOCGIFHWADDR, &if_mac) >= 0)
            {
                //index of the network device
                m_socket_address.sll_ifindex = if_idx.ifr_ifindex;
                m_socket_address.sll_halen = ETH_ALEN;

                return;
            }
            else
            {
                util::message(util::k_error, "Unable to get address of interface");
            }
        }
        else
        {
            util::message(util::k_error, "Unable to get index of interface");
        }
    }
    else
    {
        util::message(util::k_error, "Unable to open raw socket");
    }

    ::core::Engine::shutdown();
}
