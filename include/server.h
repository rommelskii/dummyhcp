//
// Created by MJ Ronduen on 1/16/26.
//

#ifndef DHCP_SERVER_H
#define DHCP_SERVER_H

#include <iostream>
#include <cstdint>
#include <ctime>
#include <string>
#include <unordered_map>

class dhcp_session {
public:
    uint32_t        xid;
    uint32_t        leased_ip;
    std::time_t     timestamp;

    dhcp_session();
    dhcp_session(uint32_t xid, uint32_t leased_ip, std::time_t timestamp);
};

class dhcp_server {
public:
    std::unordered_map<std::string, dhcp_session > sessions;
    dhcp_session find_session(const std::string& mac);
    void create_session(const std::string& mac, dhcp_session session);
};

uint32_t generate_xid();


#endif //DHCP_SERVER_H