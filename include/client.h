#ifndef DHCP_CLIENT_H
#define DHCP_CLIENT_H

/**
* file: client.h
*
* Contains clientside structures and methods. The clientside is capable of creating a DHCP session with a server (if found). 
* The clientside must keep track of its states which determine its stage in the life cycle of DHCP handling. Since DHCP requires the knowledge of MAC and
* IP addresses, the clientside must contain the necessary API handling for determining these kinds of information in supplication to the packet handling API 
* through packet.h.
*/

#include <iostream>
#include "packet.h"


class dhcp_client_context {
public:
  dhcp_state state;
  uint32_t xid;
  uint32_t yiaddr;
  uint32_t siaddr;
  uint32_t netmask;
  uint8_t mac[6];
  uint32_t lease_time;
  uint32_t renewal_time;
  uint32_t rebind_time;
  //constructors
  dhcp_client_context();

  //extractors
  dhcp_state get_state();
  uint32_t get_xid();
  uint32_t get_netmask();
  uint32_t get_yiaddr();
  uint32_t get_siaddr();
  uint8_t* get_mac();
  uint32_t get_lease_time();
  uint32_t get_renewal_time();
  uint32_t get_rebind_time();

  //mutators
  void change_state(dhcp_state new_state);
  void change_xid(uint32_t new_xid);
  void change_netmask(uint32_t new_netmask);
  void change_yiaddr(uint32_t new_yiaddr);
  void change_siaddr(uint32_t new_siaddr);
  void change_mac(uint8_t* new_mac);
  void change_lease_time(uint32_t new_lease_time);
  void change_renewal_time(uint32_t new_release_time);
  void change_rebind_time(uint32_t new_rebind_time);

  uint32_t generate_xid();

  void run_client();
};



#endif 

