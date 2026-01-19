#include <gtest/gtest.h>
#include "packet.h"
#include "packet_fixtures.h"

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
TEST_F(PacketHeaderBuild, PacketHeaderPreflightCheck) {
  dp.preflight_order_change();
  EXPECT_EQ(dp.header.xid, net_xid);
  EXPECT_EQ(dp.header.secs, net_secs);
  EXPECT_EQ(dp.header.flags, net_flags);
  EXPECT_EQ(dp.header.magic_cookie, net_magic_cookie);
}
