//
// Created by MJ Ronduen on 1/16/26.
//

#include "server.h"
#include <random>
#include <arpa/inet.h>
#include <unordered_map>

dhcp_session::dhcp_session() {
    this->leased_ip = inet_addr("0.0.0.0");
    this->timestamp = std::time(nullptr);
    this->xid = generate_xid();
}

dhcp_session::dhcp_session(uint32_t xid, uint32_t leased_ip, std::time_t timestamp) {
    this->leased_ip = leased_ip;
    this->timestamp = timestamp;
    this->xid = generate_xid();
}

dhcp_session dhcp_server::find_session(const std::string& mac) {
    if (auto it = this->sessions.find(mac); it != this->sessions.end()) {
        return it->second;
    }

    return dhcp_session{};
}

void dhcp_server::create_session(const std::string& mac, dhcp_session session) {
    this->sessions[mac] = session;
}

uint32_t generate_xid() {
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_int_distribution<uint32_t> dis(0,0xFFFFFFFF);

    return dis(gen);
}
