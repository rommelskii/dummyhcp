#ifndef PACKET_FIXTURES_H
#define PACKET_FIXTURES_H

#include "packet.h"
#include <iostream>
#include <arpa/inet.h>
#include <gtest/gtest.h>

class PacketInitialization : public ::testing::Test {
protected:
  dhcp_packet dp{};
};

class PacketHeaderBuild : public ::testing::Test {
protected:
  //header building 
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
  const uint32_t    magic_cookie  = 0x63825363;   //always

  // preflight checks
  const uint32_t net_xid = htonl(xid);
  const uint16_t net_secs = htons(secs);
  const uint16_t net_flags = htons(flags);
  const uint32_t net_magic_cookie = htonl(magic_cookie);

  void SetUp() override {
    dp.build_header(op, xid, secs, flags, ciaddr, yiaddr, siaddr, giaddr, chaddr);
  }
};

class PacketOptionsBuild : public ::testing::Test {
protected:
  dhcp_packet       discover_packet;
  dhcp_packet       offer_packet;
  dhcp_packet       request_packet;
  dhcp_packet       acknowledge_packet;

  const uint8_t     OPTION_1 = 1;
  const uint8_t     OPTION_3 = 3;
  const uint8_t     OPTION_6 = 6;
  const uint8_t     OPTION_15 = 15;
  const uint8_t     OPTION_50 = 50;
  const uint8_t     OPTION_51 = 51;
  const uint8_t     OPTION_53 = 53;
  const uint8_t     OPTION_54 = 54;
  const uint8_t     OPTION_55 = 55;
  const uint8_t     OPTION_58 = 58;
  const uint8_t     OPTION_59 = 59;
  const uint8_t     OPTION_61 = 55;

  const uint8_t     VALUE_53_DISCOVER = 1;
  const uint8_t     VALUE_53_OFFER = 2;
  const uint8_t     VALUE_53_REQUEST = 3;
  const uint8_t     VALUE_53_ACKNOWLEDGE = 5;
  const uint8_t     VALUE_53_DECLINE = 4;
  const uint8_t     VALUE_53_NAK = 6;
  const uint8_t     VALUE_53_RELEASE = 7;
  const uint8_t     VALUE_53_INFORM = 8;

  std::vector<uint8_t>   CLIENT_ID = { 0x69, 0x69, 0x69, 0x69, 0x69, 0x69 };
  const uint32_t    SERVER_IP = inet_addr("192.168.100.10");
  const uint32_t    REQUESTED_IP = inet_addr("192.168.100.10");
  const uint32_t    GATEWAY_IP = inet_addr("0.0.0.0");
  const uint32_t    SERVER_MASK = inet_addr("255.255.255.0");

  const uint32_t    LEASE_TIME = 86400;
  const uint32_t    RENEWAL_TIME = 86400;
  const uint32_t    REBINDING_TIME = 86400;

  const uint8_t    ETHERNET_TAG = 0x01;

  std::vector<uint8_t> DISCOVER_REQUEST_LIST = {OPTION_1, OPTION_3, OPTION_6, OPTION_15};
  std::vector<uint8_t> OFFER_REQUEST_LIST = {OPTION_1, OPTION_3, OPTION_6};
  std::vector<uint8_t> REQUEST_REQUEST_LIST = {OPTION_1, OPTION_3, OPTION_6, OPTION_15};

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
  const uint32_t    magic_cookie  = 0x63825363;   //always
  //
  void SetUp() override {
    discover_packet.build_header(op, xid, secs, flags, ciaddr, yiaddr, siaddr, giaddr, chaddr);
    offer_packet.build_header(op, xid, secs, flags, ciaddr, yiaddr, siaddr, giaddr, chaddr);
    request_packet.build_header(op, xid, secs, flags, ciaddr, yiaddr, siaddr, giaddr, chaddr);
    acknowledge_packet.build_header(op, xid, secs, flags, ciaddr, yiaddr, siaddr, giaddr, chaddr);
  }
};

#endif
