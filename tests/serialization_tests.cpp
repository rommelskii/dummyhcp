#include <gtest/gtest.h>
#include "packet_fixtures.h"

TEST_F(PacketHeaderBuild, PacketHeaderSerialization) {
  const size_t MAXSIZE = 1500;
  uint8_t buf[MAXSIZE];
  memset(buf, 0, sizeof(buf));

  ssize_t n = dp.serialize(buf, MAXSIZE);

  dhcp_packet serialized_dp = dhcp_packet();
  serialized_dp.deserialize(buf, n);

  EXPECT_EQ(dp.header.op, serialized_dp.header.op);
  EXPECT_EQ(dp.header.htype, serialized_dp.header.htype);
  EXPECT_EQ(dp.header.hlen, serialized_dp.header.hlen);
  EXPECT_EQ(dp.header.hops, serialized_dp.header.hops);
  EXPECT_EQ(dp.header.xid, serialized_dp.header.xid);
  EXPECT_EQ(dp.header.secs, serialized_dp.header.secs);
  EXPECT_EQ(dp.header.flags, serialized_dp.header.flags);
  EXPECT_EQ(dp.header.ciaddr, serialized_dp.header.ciaddr);
  EXPECT_EQ(dp.header.yiaddr, serialized_dp.header.yiaddr);
  EXPECT_EQ(dp.header.siaddr, serialized_dp.header.siaddr);
  EXPECT_EQ(dp.header.giaddr, serialized_dp.header.giaddr);

  for (int i=0; i<sizeof(dp.header.chaddr); ++i) {
    EXPECT_EQ(dp.header.chaddr[i], serialized_dp.header.chaddr[i]);
  }
  for (int i=0; i<sizeof(dp.header.sname); ++i) {
    EXPECT_EQ(dp.header.sname[i], serialized_dp.header.sname[i]);
  }
  for (int i=0; i<sizeof(dp.header.file); ++i) {
    EXPECT_EQ(dp.header.file[i], serialized_dp.header.file[i]);
  }

  EXPECT_EQ(dp.header.magic_cookie, serialized_dp.header.magic_cookie);
}
