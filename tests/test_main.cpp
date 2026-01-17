#include <gtest/gtest.h>
#include "packet.h"

TEST(Packet, PacketCreation) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42);
}
