#ifndef PACKET_FIXTURES_H
#define PACKET_FIXTURES_H

#include "packet.h"
#include <gtest/gtest.h>

class PacketInitialization : public ::testing::Test {
protected:
  dhcp_packet dp{};
};

class PacketHeaderBuild : public ::testing::Test {
protected:
  dhcp_packet dp;
  const uint8_t     op            = 1;     
  const uint8_t     htype         = 1;   
  const uint8_t     hlen          = 6;     
  const uint8_t     hops          = 0;      
  const uint32_t    xid           = 0xDEADBEEF;        
  const uint16_t    secs          = 2600;        
  const uint16_t    flags         = 0;       
  const uint32_t    ciaddr        = inet_addr("1.0.0.0");      
  const uint32_t    yiaddr        = inet_addr("0.1.0.0");    
  const uint32_t    siaddr        = inet_addr("0.0.1.0");     
  const uint32_t    giaddr        = inet_addr("192.168.100.69");      
        uint8_t     chaddr[16]    = {0};   
        uint8_t     sname[64]     = {0};     
        uint8_t     file[128]     = {0};      
  const uint32_t    magic_cookie  = 0xAAAAFFFF;   

  void SetUp() override {
    dp.build_header(op, xid, secs, flags, ciaddr, yiaddr, siaddr, giaddr, chaddr);
  }
};

#endif
