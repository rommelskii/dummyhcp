
#ifndef CLIENT_FIXTURES_H 
#define CLIENT_FIXTURES_H 

#include "packet.h"
#include "client.h"
#include <iostream>
#include <arpa/inet.h>
#include <gtest/gtest.h>

class ClientContextBuild : public ::testing::Test {
protected:
  dhcp_client_context dc; 

  const dhcp_state INITIAL_STATE = dhcp_state::INIT;
  const uint32_t INITIAL_XID = 0x00;
  const uint32_t INITIAL_YIADDR = inet_addr("0.0.0.0");
  const uint32_t INITIAL_SIADDR = inet_addr("0.0.0.0");
  const uint32_t INITIAL_NETMASK = inet_addr("0.0.0.0");
  std::vector<uint8_t> INITIAL_MAC = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  const uint32_t INITIAL_LEASE_TIME = 0x00;
  const uint32_t INITIAL_RENEWAL_TIME = 0x00;
  const uint32_t INITIAL_REBIND_TIME = 0x00;

  void SetUp() override {
    dc = dhcp_client_context();
  }
};

#endif
