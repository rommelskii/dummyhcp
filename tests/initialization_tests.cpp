#include <gtest/gtest.h>
#include "packet_fixtures.h"
#include "packet.h"

TEST_F(PacketInitialization, PacketInitializationCheck) {
  EXPECT_EQ(dp.header.op, 0);
  EXPECT_EQ(dp.header.htype, 0);
  EXPECT_EQ(dp.header.hlen, 0);
  EXPECT_EQ(dp.header.hops, 0);
  EXPECT_EQ(dp.header.xid, 0);
  EXPECT_EQ(dp.header.secs, 0);
  EXPECT_EQ(dp.header.flags, 0);
  EXPECT_EQ(dp.header.ciaddr, 0);
  EXPECT_EQ(dp.header.yiaddr, 0);
  EXPECT_EQ(dp.header.siaddr, 0);
  EXPECT_EQ(dp.header.giaddr, 0);
  for ( auto x : dp.header.chaddr ) {
    EXPECT_EQ(x, 0);
  }
  for ( auto x : dp.header.sname ) {
    EXPECT_EQ(x, 0);
  }
  for ( auto x : dp.header.file ) {
    EXPECT_EQ(x, 0);
  }
  EXPECT_EQ(dp.header.magic_cookie, 0);
}

