//
// Created by MJ Ronduen on 1/14/26.
//

#include "dhcp_packet.h"

#include <iomanip>
#include <iostream> //for cerr
#include <random>
#include <vector>
#include <sstream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>

void dhcp_packet::packet_from_stream(uint8_t buffer[MAXSIZE], ssize_t bytes_received) {
    if (bytes_received >= sizeof(header)) {
        std::memcpy(&this->header, buffer, sizeof(header));
    }
    //begin options field extraction
    uint8_t *options_start = buffer + sizeof(header); // option field starts after header
    uint8_t *current = options_start;

    while (current < (buffer + bytes_received)) {
        //buffer + bytes_received is end of buffered packet itself
        uint8_t type = *current;

        if (type == 255) break;
        if (type == 0) {
            current++;
            continue;
        }
        uint8_t len = *(current + 1);
        if (current + 2 + len > buffer + bytes_received) {
            std::cerr << "error: inconsistent length value\n";
            break;
        }

        this->options[type].clear(); // ensures that we are writing up-to-date information (if option exists already)
        for (int i = 0; i < len; ++i) {
            this->options[type].push_back(*(current + 2 + i));
        }

        current += (2 + len);
    }

    // revert all network endianness to host order
}

void dhcp_packet::build_client_header(uint32_t ciaddr, uint32_t giaddr, uint32_t xid, uint8_t mac[8]) {
    dhcp_header dp = dhcp_header();
    dp.op = 1;
    dp.htype = 1;
    dp.hlen = 6;
    dp.hops = 0;
    dp.xid = xid;
    dp.secs = 20000;
    dp.flags = 1;
    dp.siaddr = inet_addr("0.0.0.0"); // out of project scope

    dp.ciaddr = ciaddr;
    dp.giaddr = giaddr;

    std::memcpy(dp.chaddr, mac, 8);
    std::memset(dp.sname, 0, sizeof(dp.sname));
    std::memset(dp.file, 0, sizeof(dp.file));

    dp.magic_cookie = 0x63825363;

    this->header = dp;
}
ssize_t dhcp_packet::serialize(uint8_t buf[], ssize_t max_len) {
    // Ensure we don't overflow the buffer
    if (max_len < sizeof(dhcp_header) + 4) return -1;

    // Create a temporary header to hold network-ordered values
    dhcp_header net_hdr = this->header;

    // Convert multi-byte fields to Network Byte Order
    net_hdr.xid    = htonl(this->header.xid);
    net_hdr.secs   = htons(this->header.secs);
    net_hdr.flags  = htons(this->header.flags);
    net_hdr.ciaddr = htonl(this->header.ciaddr);
    net_hdr.yiaddr = htonl(this->header.yiaddr);
    net_hdr.siaddr = htonl(this->header.siaddr);
    net_hdr.giaddr = htonl(this->header.giaddr);

    // Copy the converted header to the buffer
    std::memcpy(buf, &net_hdr, sizeof(dhcp_header));

    // Don't forget the Magic Cookie (must be 0x63825363 in network order)
    uint32_t cookie = htonl(0x63825363);
    size_t offset = sizeof(dhcp_header);
    std::memcpy(buf + offset, &cookie, 4);

    for (auto const& [type, value] : this->options) {
        if (offset + 2 + value.size() > (size_t)max_len) return - 1;

        buf[offset++] = type;
        buf[offset++] = (uint8_t)value.size();

        std::memcpy(buf + offset, value.data(), value.size());
        offset += value.size();
    }

    if (offset < (size_t)max_len) {
        buf[offset++] = 0xFF;
    } else {
        return -1;
    }

    return (ssize_t)offset;
}

void dhcp_packet::print() {
    std::cout << "\n========= DHCP PACKET CONTENTS =========" << std::endl;

    // 1. Print Basic Header Info
    std::cout << "OP:    " << (int)header.op << (header.op == 1 ? " (BOOTREQUEST)" : " (BOOTREPLY)") << std::endl;
    std::cout << "XID:   0x" << std::hex << ntohl(header.xid) << std::dec << std::endl;

    // 2. Print IP Addresses
    // We use in_addr struct so inet_ntoa can format the uint32_t
    struct in_addr addr;

    addr.s_addr = htonl(header.ciaddr); // Already in network order if filled correctly
    std::cout << "CIADDR: " << inet_ntoa(addr) << std::endl;

    addr.s_addr = htonl(header.yiaddr);
    std::cout << "YIADDR: " << inet_ntoa(addr) << std::endl;

    addr.s_addr = htonl(header.giaddr);
    std::cout << "GIADDR: " << inet_ntoa(addr) << std::endl;

    // 3. Print MAC Address (CHADDR)
    std::cout << "CHADDR: ";
    for(int i = 0; i < 6; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)header.chaddr[i] << (i < 5 ? ":" : "");
    }
    std::cout << std::dec << std::setfill(' ') << std::endl;

    // 4. Print Options
    std::cout << "OPTIONS (" << options.size() << " found):" << std::endl;
    for (auto const& [type, value] : options) {
        std::cout << "  [Type " << (int)type << "] Len: " << value.size() << " | Data: ";
        for (uint8_t byte : value) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
        }
        std::cout << std::dec << std::endl;
    }
    std::cout << "========================================" << std::endl;
}

std::string mac_to_string(uint8_t mac[16]) {
    std::stringstream ss;
    for (int i=0;i<6;++i) {
        ss << std::hex
           << std::setw(2)
           << std::setfill('0')
           << static_cast<int>(mac[i]);
        if (i < 5) ss << ":";
    }
    return ss.str();
}

