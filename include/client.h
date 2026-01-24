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
#include <vector>
#include <netinet/in.h>
#include "packet.h"


class dhcp_client_context {
public:
  int sockfd;
  dhcp_state state;
  uint32_t xid;
  uint32_t yiaddr;
  uint32_t siaddr;
  uint32_t ciaddr;
  uint32_t giaddr;
  uint32_t netmask;
  uint32_t server_id;
  std::vector<uint8_t> mac;
  uint32_t lease_time;
  uint32_t renewal_time;
  uint32_t rebind_time;

  //packet buffer
  sockaddr_in inbound_packet; 
  uint8_t packet_buf[MAXLINE];
  std::vector<uint8_t> requested_options;

  //constructors
  dhcp_client_context();

  //extractors
  dhcp_state get_state();
  uint32_t get_xid();
  uint32_t get_netmask();
  uint32_t get_yiaddr();
  uint32_t get_siaddr();
  uint32_t get_ciaddr();
  uint32_t get_giaddr();
  uint32_t get_server_id();

  std::vector<uint8_t> get_mac();

  uint32_t get_lease_time();
  uint32_t get_renewal_time();
  uint32_t get_rebind_time();

  //mutators
  void change_state(dhcp_state new_state);
  void change_xid(uint32_t new_xid);
  void change_netmask(uint32_t new_netmask);
  void change_yiaddr(uint32_t new_yiaddr);
  void change_siaddr(uint32_t new_siaddr);
  void change_ciaddr(uint32_t new_ciaddr);
  void change_giaddr(uint32_t new_giaddr);
  void change_server_id(uint32_t new_server_ip);
  void change_mac(std::vector<uint8_t> new_mac);
  void change_lease_time(uint32_t new_lease_time);
  void change_renewal_time(uint32_t new_release_time);
  void change_rebind_time(uint32_t new_rebind_time);

  uint32_t generate_xid();

  void run_client();

  void build_client_header(dhcp_packet& discovery_packet);

  void receive_offer(dhcp_packet& offer_packet);
  void receive_acknowledge(dhcp_packet acknowledge_packet);

  int validate_offer(dhcp_packet& offer_packet);
  int validate_acknowledge(dhcp_packet& acknowledge_packet);

  void perform_broadcast(dhcp_packet discovery_packet);
  void perform_request(dhcp_packet request_packet);

  //void unpack_offer(dhcp_packet offer_packet); <- i think we can combine this with validate


  void bind_ip();
};



#endif 

