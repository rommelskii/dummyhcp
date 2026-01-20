#include <iostream>
#include <cstdint>
#include <random>
#include "unistd.h"

dhcp_client_context();

dhcp_state dhcp_client_context::get_state() {
  return this->state;
}

uint32_t dhcp_client_context::get_xid() {
  return this->xid;
}

uint32_t dhcp_client_context::get_netmask() {
  return this->netmask;
}

uint32_t dhcp_client_context::get_yiaddr() {
  return this->yiaddr;  
}

uint32_t dhcp_client_context::get_siaddr() {
  return this->siaddr;
}

uint8_t* dhcp_client_context::get_mac() {
  return this->mac;
}

uint32_t dhcp_client_context::get_lease_time() {
  return this->lease_time;
}

uint32_t dhcp_client_context::get_renewal_time() {
  return this->renewal_time;
}

uint32_t dhcp_client_context::get_rebind_time() {
  return this->rebind_time;
}

//mutators
void dhcp_client_context::change_state(dhcp_state new_state) {
  this->state = new_state;
}

void dhcp_client_context::change_xid(uint32_t new_xid) {
  this->xid = new_xid;
}

void dhcp_client_context::change_netmask(uint32_t new_netmask) {
  this->netmask = new_netmask;
}

void dhcp_client_context::change_yiaddr(uint32_t new_yiaddr) {
  this->yiaddr = new_yiaddr;
}

void dhcp_client_context::change_siaddr(uint32_t new_siaddr) {
  this->siaddr = new_siaddr;
}

void dhcp_client_context::change_mac(uint8_t* new_mac) {
  std::memcpy(this->mac, new_mac, sizeof(new_mac));
}

void dhcp_client_context::change_lease_time(uint32_t new_lease_time) {
  this->lease_time = new_lease_time;
}

void dhcp_client_context::change_renewal_time(uint32_t new_release_time) {
  this->release_time = new_release_time;
}

void change_rebind_time(uint32_t new_rebind_time) {
  this->rebind_time = new_rebind_time;
}

uint32_t generate_xid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);
    return dis(gen);
}

void dhcp_client_context::run_client() {
  //socket binding logic
  //all socket variables go here
  const int DUMMYHCP_PORT = 8069;
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
  int broadcast_enable = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
    perror("setsockopt failed");
    return;
  }

  sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_BROADCAST;
  servaddr.sin_port = htons(DUMMYHCP_PORT);

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
    perror("bind failed");
    return;
  }

  //begin business logic
  while (true) {
    dhcp_state current_state = this->get_state();
    switch (current_state) {
      case dhcp_state::INIT:
        break;
      case dhcp_state::SELECTING:
        break;
      case dhcp_state::REQUESTING:
        break;
      case dhcp_state::BOUND:
        break;
      case dhcp_state::RENEWING:
        break;
    }
  }

  close(sockfd);
}
