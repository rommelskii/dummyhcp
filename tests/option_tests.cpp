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

  //check option vectors exist
  auto op53_vec = options[OPTION_53];
  auto op55_vec = options[OPTION_55];
  ASSERT_NE(op53_vec, options.end());
  ASSERT_NE(op55_vec, options.end());

  //insert payload and check
  op53_vec.push_back(VALUE_53_DISCOVER);
  ASSERT_EQ(op53_vec[0], VALUE_53_DISCOVER);

  for (auto x : DISCOVER_REQUEST_LIST) {
    op55_vec.push_back(x);
  }
  ASSERT_EQ(op55_vec.size(), DISCOVER_REQUEST_LIST.size());
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
  ASSERT_NE(op53_vec, options.end());
  ASSERT_NE(op54_vec, options.end());
  ASSERT_NE(op51_vec, options.end());
  ASSERT_NE(op58_vec, options.end());
  ASSERT_NE(op59_vec, options.end());
  ASSERT_NE(op55_vec, options.end());


  op53_vec.push_back(VALUE_53_OFFER);
  ASSERT_EQ(op53_vec[0], VALUE_53_OFFER);

  offer_packet.pack_ip(op54_vec, SERVER_IP);
  ASSERT_EQ(op54_vec.size(), PACK32_TO_8);

  const size_t PACK32_TO_8 = sizeof(LEASE_TIME) / sizeof(uint8_t);
  offer_packet.pack_time(op51_vec, LEASE_TIME);
  ASSERT_EQ(op51_vec.size(), PACK32_TO_8);

  offer_packet.pack_time(op58_vec, RENEWAL_TIME);
  ASSERT_EQ(op58_vec.size(), PACK32_TO_8);

  offer_packet.pack_time(op59_vec, REBINDING_TIME);
  ASSERT_EQ(op59_vec.size(), PACK32_TO_8);

  for (auto x : OFFER_REQUEST_LIST) {
    op55_vec.push_back(x);
  }
  ASSERT_EQ(op55_vec.size(), OFFER_REQUEST_LIST.size());
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
  ASSERT_NE(op53_vec, options.end());
  ASSERT_NE(op50_vec, options.end());
  ASSERT_NE(op54_vec, options.end());
  ASSERT_NE(op55_vec, options.end());
  ASSERT_NE(op61_vec, options.end());

  op53_vec.push_back(VALUE_53_REQUEST);
  ASSERT_EQ(op53_vec[0], VALUE_53_REQUEST);

  request_packet.pack_ip(op50_vec, REQUESTED_IP);
  ASSERT_EQ(op50_vec.size(), PACK32_TO_8);

  request_packet.pack_ip(op54_vec, SERVER_IP);
  ASSERT_EQ(op54_vec.size(), PACK32_TO_8);

  for (auto x : REQUEST_REQUEST_LIST) {
    op55_vec.push_back(x);
  }
  ASSERT_EQ(op55_vec.size(), REQUEST_REQUEST_LIST.size());

  for (auto x : CLIENT_ID) {
    op61_vec.push_back(x);
  }
  op61_vec.push_back(ETHERNET_TAG); // request list size + 1
  ASSERT_EQ(op61_vec.size(), REQUEST_REQUEST_LIST.size() + 1);
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
  ASSERT_NE(op53_vec, options.end());
  ASSERT_NE(op51_vec, options.end());
  ASSERT_NE(op54_vec, options.end());
  ASSERT_NE(op1_vec, options.end());
  ASSERT_NE(op3_vec, options.end());

  op53_vec.push_back(VALUE_53_ACKNOWLEDGE);
  ASSERT_EQ(op53_vec[0], VALUE_53_ACKNOWLEDGE);

  acknowledge_packet.pack_ip(op54_vec, SERVER_IP);
  ASSERT_EQ(op54_vec.size(), PACK32_TO_8);

  acknowledge_packet.pack_time(op51_vec, LEASE_TIME);
  ASSERT_EQ(op51_vec.size(), PACK32_TO_8);

  acknowledge_packet.pack_ip(op1_vec, SERVER_MASK);
  ASSERT_EQ(op1_vec.size(), PACK32_TO_8);

  acknowledge_packet.pack_ip(op3_vec, GATEWAY_IP);
  ASSERT_EQ(op3_vec.size(), PACK32_TO_8);
}
