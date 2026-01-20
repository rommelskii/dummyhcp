#include <gtest/gtest.h>
#include "client.h"
#include "client_fixtures.h"
#include "packet.h"

TEST_F(ClientContextBuild, ConstructorTest) {
  ASSERT_EQ(dc.state, INITIAL_STATE);  
  ASSERT_EQ(dc.xid, INITIAL_XID);  
  ASSERT_EQ(dc.yiaddr, INITIAL_YIADDR);  
  ASSERT_EQ(dc.siaddr, INITIAL_SIADDR);  
  ASSERT_EQ(dc.netmask, INITIAL_NETMASK);  
  for (int i=0; i<sizeof(INITIAL_MAC); ++i) {
    ASSERT_EQ(dc.mac[i], INITIAL_MAC[i]);
  }
  ASSERT_EQ(dc.lease_time, INITIAL_LEASE_TIME);  
  ASSERT_EQ(dc.renewal_time, INITIAL_RENEWAL_TIME);  
  ASSERT_EQ(dc.rebind_time, INITIAL_REBIND_TIME);  
}

TEST_F(ClientContextBuild, GetterTest) {
  ASSERT_EQ(dc.get_state(), INITIAL_STATE);  
  ASSERT_EQ(dc.get_xid(), INITIAL_XID);  
  ASSERT_EQ(dc.get_yiaddr(), INITIAL_YIADDR);  
  ASSERT_EQ(dc.get_siaddr(), INITIAL_SIADDR);  
  ASSERT_EQ(dc.get_netmask(), INITIAL_NETMASK);  

  std::vector<uint8_t> client_mac = dc.get_mac();
  for (int i=0; i<sizeof(client_mac); ++i) {
    ASSERT_EQ(client_mac[i], INITIAL_MAC[i]);
  }

  ASSERT_EQ(dc.get_lease_time(), INITIAL_LEASE_TIME);  
  ASSERT_EQ(dc.get_renewal_time(), INITIAL_RENEWAL_TIME);  
  ASSERT_EQ(dc.get_rebind_time(), INITIAL_REBIND_TIME);  
}

TEST_F(ClientContextBuild, MutatorTest) {
  const dhcp_state NEW_STATE = dhcp_state::SELECTING;
  const uint32_t NEW_XID = 0x69696969;
  const uint32_t NEW_YIADDR = inet_addr("192.168.0.23");
  const uint32_t NEW_SIADDR = inet_addr("192.168.0.10");
  const uint32_t NEW_NETMASK = inet_addr("255.255.255.0");
  std::vector<uint8_t> NEW_MAC = {0x04, 0xFF, 0x69, 0x22, 0x12, 0x53};
  const uint32_t NEW_LEASE_TIME = 86400;
  const uint32_t NEW_RENEWAL_TIME = 42000;
  const uint32_t NEW_REBIND_TIME = 2600;

  dc.change_state(NEW_STATE);
  dc.change_xid(NEW_XID);
  dc.change_yiaddr(NEW_YIADDR);
  dc.change_siaddr(NEW_SIADDR);
  dc.change_netmask(NEW_NETMASK);
  dc.change_lease_time(NEW_LEASE_TIME);
  dc.change_renewal_time(NEW_RENEWAL_TIME);
  dc.change_rebind_time(NEW_REBIND_TIME);
  dc.change_mac(NEW_MAC);

  ASSERT_EQ(dc.state, NEW_STATE);  
  ASSERT_EQ(dc.xid, NEW_XID);  
  ASSERT_EQ(dc.yiaddr, NEW_YIADDR);  
  ASSERT_EQ(dc.siaddr, NEW_SIADDR);  
  ASSERT_EQ(dc.netmask, NEW_NETMASK);  
  for (int i=0; i<NEW_MAC.size(); ++i) {
    ASSERT_EQ(dc.mac[i], NEW_MAC[i]);
  }
  ASSERT_EQ(dc.lease_time, NEW_LEASE_TIME);  
  ASSERT_EQ(dc.renewal_time, NEW_RENEWAL_TIME);  
  ASSERT_EQ(dc.rebind_time, NEW_REBIND_TIME);  
}

TEST_F(ClientContextBuild, OperationalTest) {
  
}
