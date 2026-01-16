//
// Created by MJ Ronduen on 1/14/26.
//

#include "dhcp_packet.h"

#include <iomanip>
#include <iostream> //for cerr
#include <random>
#include <vector>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>

void dhcp_packet::packet_from_stream(uint8_t buffer[MAXSIZE], ssize_t bytes_received) {
    if (bytes_received >= sizeof(header)) {
        std::memcpy(&this->header, buffer, sizeof(header));
    }
    //begin options field extraction
    uint8_t *options_start = buffer + sizeof(header); // option field starts after header
    uint8_t *current = options_start;

    current += 4; // skip 4 bytes (magic cookie)
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

ssize_t dhcp_packet::serialize(uint8_t buf[MAXSIZE], ssize_t max_len) {
    if (max_len > sizeof(dhcp_packet) + 4) return -1;
    //copy the dhcp header from the class
    ssize_t offset = 0;
    std::memcpy(buf, &this->header, sizeof(this->header));
    offset += sizeof(dhcp_packet);

    //iterate over the options map in the class
    for (const auto& [tag, value] : this->options) {
        if (offset + 2 > max_len) return -1; // prevents overflow from the value field

        buf[offset++] = tag;                   // tag value
        buf[offset++] = static_cast<uint8_t>(value.size()); // length value

        std::memcpy(buf + offset, value.data(), value.size()); // copy the value alongside its length
        offset += static_cast<ssize_t>(value.size());
    }

    return offset;
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

