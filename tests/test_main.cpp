#include <gtest/gtest.h>
#include "packet.h"
#include "arpa/inet.h"
#include "packet_fixtures.h"

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

TEST_F(PacketHeaderBuild, PacketHeaderOpCheck) {
  EXPECT_EQ(dp.header.op, op);
}
TEST_F(PacketHeaderBuild, PacketHeaderXidCheck) {
  EXPECT_EQ(dp.header.xid, xid);
}
TEST_F(PacketHeaderBuild, PacketHeaderSecsCheck) {
  EXPECT_EQ(dp.header.secs, secs);
}
TEST_F(PacketHeaderBuild, PacketHeaderFlagsCheck) {
  EXPECT_EQ(dp.header.flags, flags);
}
TEST_F(PacketHeaderBuild, PacketHeaderCiaddrCheck) {
  EXPECT_EQ(dp.header.ciaddr, ciaddr);
}
TEST_F(PacketHeaderBuild, PacketHeaderYiaddrCheck) {
  EXPECT_EQ(dp.header.yiaddr, yiaddr);
}
TEST_F(PacketHeaderBuild, PacketHeaderSiaddrCheck) {
  EXPECT_EQ(dp.header.siaddr, siaddr);
}
TEST_F(PacketHeaderBuild, PacketHeaderGiaddrCheck) {
  EXPECT_EQ(dp.header.giaddr, giaddr);
}
TEST_F(PacketHeaderBuild, PacketHeaderChaddrCheck) {
  for(int i=0; i<sizeof(dp.header.chaddr); ++i) {
    EXPECT_EQ(dp.header.chaddr[i], chaddr[i]);
  }
}
TEST_F(PacketHeaderBuild, PacketHeaderSnameCheck) {
  for(int i=0; i<sizeof(dp.header.sname); ++i) {
    EXPECT_EQ(dp.header.sname[i], sname[i]);
  }
}
TEST_F(PacketHeaderBuild, PacketHeaderFileCheck) {
  for(int i=0; i<sizeof(dp.header.file); ++i) {
    EXPECT_EQ(dp.header.file[i], file[i]);
  }
}
