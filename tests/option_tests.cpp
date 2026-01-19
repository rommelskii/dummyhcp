#include <gtest/gtest.h>
#include "packet.h"
#include "packet_fixtures.h"

/**
 * file: options_test.cpp
 *
 * Contains unit tests for options population
 */

TEST_F(PacketOptionsBuild, DhcpDiscoverTest) {
  /**
  * Test: Packet Discover
  * Creates a DHCPDISCOVER packet
  */
  //prepopulate DHCPDISCOVER payload
  discover_packet.add_option(OPTION_53);
  discover_packet.add_option(OPTION_55);

  auto options = discover_packet.options;

  auto op53_vec = options[OPTION_53];
  auto op55_vec = options[OPTION_55];

  //insert payload
  op53_vec.push_back(VALUE_53_DISCOVER);
  //op55_vec.insert(op55_vec.end(), DISCOVER_REQUEST_LIST.begin(), DISCOVER_REQUEST_LIST.end());
  for (auto x : DISCOVER_REQUEST_LIST) {
    op55_vec.push_back(x);
  }
}

TEST_F(PacketOptionsBuild, DhcpOfferTest) {
  /**
  * Test: Packet Offer 
  * Creates a DHCPOFFER packet 
  */
  //prepopulate DHCPOFFER payload
  offer_packet.add_option(OPTION_53); 
  offer_packet.add_option(OPTION_54); 
  offer_packet.add_option(OPTION_51); 
  offer_packet.add_option(OPTION_58); 
  offer_packet.add_option(OPTION_59); 
  offer_packet.add_option(OPTION_55); 

  auto options = offer_packet.options;
  auto op53_vec = options[OPTION_53];
  auto op54_vec = options[OPTION_54];
  auto op51_vec = options[OPTION_51];
  auto op58_vec = options[OPTION_58];
  auto op59_vec = options[OPTION_59];
  auto op55_vec = options[OPTION_55];

  op53_vec.push_back(VALUE_53_OFFER);
  offer_packet.pack_ip(op54_vec, SERVER_IP);
  offer_packet.pack_time(op51_vec, LEASE_TIME);
  offer_packet.pack_time(op58_vec, RENEWAL_TIME);
  offer_packet.pack_time(op59_vec, REBINDING_TIME);
  //op55_vec.insert(op55_vec.end(), OFFER_REQUEST_LIST.begin(), OFFER_REQUEST_LIST.end());
  for (auto x : OFFER_REQUEST_LIST) {
    op55_vec.push_back(x);
  }
}

TEST_F(PacketOptionsBuild, DhcpRequestTest) {
  /**
  * Test: Packet Offer 
  * Creates a DHCPOFFER packet 
  */

  request_packet.add_option(OPTION_53);
  request_packet.add_option(OPTION_50);
  request_packet.add_option(OPTION_54);
  request_packet.add_option(OPTION_55);
  request_packet.add_option(OPTION_61);
  
  auto options = request_packet.options;

  auto op53_vec = options[OPTION_53];
  auto op50_vec = options[OPTION_50];
  auto op54_vec = options[OPTION_54];
  auto op55_vec = options[OPTION_55];
  auto op61_vec = options[OPTION_61];

  op53_vec.push_back(VALUE_53_REQUEST);
  request_packet.pack_ip(op50_vec, REQUESTED_IP);
  request_packet.pack_ip(op54_vec, SERVER_IP);
  for (auto x : REQUEST_REQUEST_LIST) {
    op55_vec.push_back(x);
  }
  for (auto x : CLIENT_ID) {
    op55_vec.push_back(x);
  }
  op61_vec.push_back(ETHERNET_TAG);
}

TEST_F(PacketOptionsBuild, DhcpAcknowledgeTest) {
  /**
  * Test: Packet Offer 
  * Creates a DHCPACKNOWLEDGE packet 
  */
  acknowledge_packet.add_option(OPTION_53);
  acknowledge_packet.add_option(OPTION_54);
  acknowledge_packet.add_option(OPTION_51);
  acknowledge_packet.add_option(OPTION_1);
  acknowledge_packet.add_option(OPTION_3);

  auto options = acknowledge_packet.options;

  auto op53_vec = options[OPTION_53];
  auto op51_vec = options[OPTION_51];
  auto op54_vec = options[OPTION_54];
  auto op1_vec = options[OPTION_1];
  auto op3_vec = options[OPTION_3];

  op53_vec.push_back(VALUE_53_ACKNOWLEDGE);
  acknowledge_packet.pack_ip(op54_vec, SERVER_IP);
  acknowledge_packet.pack_time(op51_vec, LEASE_TIME);
  acknowledge_packet.pack_ip(op51_vec, SERVER_MASK);
  acknowledge_packet.pack_ip(op3_vec, GATEWAY_IP);
}
