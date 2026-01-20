#include <iostream>
#include <cstdint>
#include <random>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "unistd.h"
#include "client.h"

dhcp_client_context::dhcp_client_context() {
  this->state = dhcp_state::INIT;
  this->xid = 0x00;
  this->netmask = inet_addr("0.0.0.0");
  this->yiaddr = inet_addr("0.0.0.0");
  this->siaddr = inet_addr("0.0.0.0");
  this->giaddr = inet_addr("0.0.0.0");
  this->netmask = inet_addr("0.0.0.0");
  this->mac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  this->lease_time = 0x00;
  this->renewal_time = 0x00;
  this->rebind_time = 0x00;
}

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

uint32_t dhcp_client_context::get_giaddr() {
  return this->giaddr;
}

std::vector<uint8_t> dhcp_client_context::get_mac() {
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

void dhcp_client_context::change_giaddr(uint32_t new_giaddr) {
  this->giaddr = new_giaddr;
}

void dhcp_client_context::change_mac(std::vector<uint8_t> new_mac) {
  this->mac = new_mac;
}

void dhcp_client_context::change_lease_time(uint32_t new_lease_time) {
  this->lease_time = new_lease_time;
}

void dhcp_client_context::change_renewal_time(uint32_t new_renewal_time) {
  this->renewal_time = new_renewal_time;
}

void dhcp_client_context::change_rebind_time(uint32_t new_rebind_time) {
  this->rebind_time = new_rebind_time;
}

uint32_t generate_xid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);
    return dis(gen);
}

void dhcp_client_context::run_client() {
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

  // client metadata
  // note: replace these with actual syscall for extracting the relevant information
  std::vector<uint8_t> fake_mac = {0x69, 0x69, 0x69, 0x69, 0x69, 0x69};
  uint32_t session_xid = this->generate_xid();
  uint8_t mac_buffer[16];
  std::memset(mac_buffer, 0x00, 16);
  for (int i=0; i<6; ++i) {
    mac_buffer[i] = fake_mac[i];
  }

  // packet buffer for serialization
  uint8_t packet_buf[MAXLINE];
  std::memset(packet_buf, 0x00, MAXLINE);

  //begin business logic
  while (true) {
    dhcp_state current_state = this->get_state();
    switch (current_state) {
      case dhcp_state::INIT:
        dhcp_packet broadcast_discovery = dhcp_packet();
        broadcast_discovery.build_header(BOOTREQUEST, session_xid, this->get_lease_time(), BROADCAST_FLAG,
                                         this->get_ciaddr(), this->get_yiaddr(), this->get_siaddr(), this->get_giaddr(),
                                         mac_buffer);
        broadcast_discovery.serialize(packet_buf, MAXLINE);
        broadcast_discovery.preflight_order_change();
        //ssize_t bytes_sent = sendto(sockfd, packet_buf, MAXLINE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
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
