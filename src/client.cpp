#include <iostream>
#include <cstdint>
#include <random>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <algorithm>
#include "unistd.h"
#include "client.h"
#include "packet.h"

dhcp_client_context::dhcp_client_context() {
  this->state = dhcp_state::INIT;
  this->xid = 0x00;
  this->netmask = inet_addr("0.0.0.0");
  this->yiaddr = inet_addr("0.0.0.0");
  this->siaddr = inet_addr("0.0.0.0");
  this->ciaddr = inet_addr("0.0.0.0");
  this->giaddr = inet_addr("0.0.0.0");
  this->netmask = inet_addr("0.0.0.0");
  this->mac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  this->lease_time = 0x00;
  this->renewal_time = 0x00;
  this->rebind_time = 0x00;
  
  std::memset(this->packet_buf, 0x00, MAXLINE);
}

dhcp_state dhcp_client_context::get_state() {
  return this->state;
}

uint32_t dhcp_client_context::get_xid() {
  return this->xid;
}

uint32_t dhcp_client_context::get_netmask() {
  return this->netmask;
}

uint32_t dhcp_client_context::get_yiaddr() {
  return this->yiaddr;  
}

uint32_t dhcp_client_context::get_siaddr() {
  return this->siaddr;
}

uint32_t dhcp_client_context::get_ciaddr() {
  return this->ciaddr;
}

uint32_t dhcp_client_context::get_giaddr() {
  return this->giaddr;
}

uint32_t dhcp_client_context::get_server_id() {
  return this->server_id;
}

std::vector<uint8_t> dhcp_client_context::get_mac() {
  return this->mac;
}

uint32_t dhcp_client_context::get_lease_time() {
  return this->lease_time;
}

uint32_t dhcp_client_context::get_renewal_time() {
  return this->renewal_time;
}

uint32_t dhcp_client_context::get_rebind_time() {
  return this->rebind_time;
}

//mutators
void dhcp_client_context::change_state(dhcp_state new_state) {
  this->state = new_state;
}

void dhcp_client_context::change_xid(uint32_t new_xid) {
  this->xid = new_xid;
}

void dhcp_client_context::change_netmask(uint32_t new_netmask) {
  this->netmask = new_netmask;
}

void dhcp_client_context::change_yiaddr(uint32_t new_yiaddr) {
  this->yiaddr = new_yiaddr;
}

void dhcp_client_context::change_siaddr(uint32_t new_siaddr) {
  this->siaddr = new_siaddr;
}

void dhcp_client_context::change_ciaddr(uint32_t new_ciaddr) {
  this->ciaddr = new_ciaddr;
}

void dhcp_client_context::change_giaddr(uint32_t new_giaddr) {
  this->giaddr = new_giaddr;
}

void dhcp_client_context::change_server_id(uint32_t new_server_id) {
  this->server_id = new_server_id;
}

void dhcp_client_context::change_mac(std::vector<uint8_t> new_mac) {
  this->mac = new_mac;
}

void dhcp_client_context::change_lease_time(uint32_t new_lease_time) {
  this->lease_time = new_lease_time;
}

void dhcp_client_context::change_renewal_time(uint32_t new_renewal_time) {
  this->renewal_time = new_renewal_time;
}

void dhcp_client_context::change_rebind_time(uint32_t new_rebind_time) {
  this->rebind_time = new_rebind_time;
}

uint32_t dhcp_client_context::generate_xid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);
    return dis(gen);
}

void dhcp_client_context::run_client() {
  this->sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
  int broadcast_enable = 1;

  if (setsockopt(this->sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
    perror("setsockopt failed");
    return;
  }

  sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_BROADCAST;
  servaddr.sin_port = htons(DUMMYHCP_PORT);

  if (bind(this->sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
    perror("bind failed");
    return;
  }

  // client metadata
  // note: replace these with actual syscall for extracting the relevant information
  std::vector<uint8_t> fake_mac = {0x69, 0x69, 0x69, 0x69, 0x69, 0x69};
  uint32_t session_xid = this->generate_xid();
  uint8_t mac_buffer[16];
  std::memset(mac_buffer, 0x00, 16);
  for (int i=0; i<6; ++i) {
    mac_buffer[i] = fake_mac[i];
  }


  //begin business logic
  while (true) {
    dhcp_state current_state = this->get_state();
    switch (current_state) {
      case dhcp_state::INIT: {
        // perform broadcast
        dhcp_packet broadcast_discovery;
        this->build_client_header(broadcast_discovery);
        this->perform_broadcast(broadcast_discovery);
        // transition
        this->change_state(dhcp_state::SELECTING);
        break;
      }
      case dhcp_state::SELECTING: {
        // receive offer
        dhcp_packet offer_reply;
        this->receive_offer(offer_reply);

        // verify first reply it receives
        if (this->validate_offer(offer_reply) == -1) {
          std::cerr << "Error: malformed offer reply";
          this->change_state(dhcp_state::INIT);
        }

        // transition
        this->change_state(dhcp_state::REQUESTING);
        break;
      }
      case dhcp_state::REQUESTING: {
        // submit data to specified server in recent DHCPOFFER 
        dhcp_packet server_request;
        this->perform_request(server_request);
        // await acknowledge reply and validate
        dhcp_packet ack_reply;
        this->receive_acknowledge(ack_reply);
        if (this->validate_acknowledge(ack_reply) == 0) {
          std::cerr << "Error: malformed acknowledge reply";
          this->change_state(dhcp_state::INIT);
        }
        // transition to BOUND
        this->change_state(dhcp_state::BOUND);
        break;
      }
      case dhcp_state::BOUND: {
        // apply to clientside 
        this->bind_ip();
        break;
      case dhcp_state::RENEWING:
        // insert renewal logic here
        break;
      }
    }
  }

  close(sockfd);
}

void dhcp_client_context::build_client_header(dhcp_packet& discovery_packet) {
  discovery_packet.build_header(1, //BOOTREQUEST
                                this->get_xid(),
                                this->get_lease_time(),
                                0x8000,
                                this->get_ciaddr(),
                                this->get_yiaddr(),
                                this->get_siaddr(),
                                this->get_giaddr(),
                                this->get_mac().data()); 
  return;
}

void dhcp_client_context::receive_offer(dhcp_packet& offer_packet) {
  socklen_t addr_len = sizeof(this->inbound_packet);
  memset(&(this->inbound_packet), 0, addr_len);

  ssize_t bytes_received = recvfrom(this->sockfd, this->packet_buf, MAXLINE, 0, (struct sockaddr*)&(this->inbound_packet), &addr_len);

  if (bytes_received < 0) {
    std::cerr << "Error: failed to receive bytes from sockfd";
    return;
  }

  offer_packet.deserialize(this->packet_buf, bytes_received);
  return;
}

void dhcp_client_context::receive_acknowledge(dhcp_packet acknowledge_packet) {
  socklen_t addr_len = sizeof(this->inbound_packet);
  memset(&(this->inbound_packet), 0, addr_len);

  ssize_t bytes_received = recvfrom(this->sockfd, this->packet_buf, MAXLINE, 0, (struct sockaddr*)&(this->inbound_packet), &addr_len);

  if (bytes_received < 0) {
    std::cerr << "Error: failed to receive bytes from sockfd";
    return;
  }

  acknowledge_packet.deserialize(this->packet_buf, bytes_received);
  return;
}

int dhcp_client_context::validate_offer(dhcp_packet& offer_packet) {
  std::unordered_map<uint8_t, std::vector<uint8_t>>& offer_options = offer_packet.options;

  if (offer_packet.header.xid != this->xid) {
    std::cerr << "DHCPOFFER error: XID mismatch. Expected " << this->xid 
      << " but got " << offer_packet.header.xid << std::endl;
    return -1;
  }

  auto it_53 = offer_options.find(53);
  if (it_53 == offer_options.end() || it_53->second.empty()) {
    std::cerr << "DHCPOFFER error: could not find option 53" << std::endl;
    return -1;
  }
  if (it_53->second[0] != 2) { // 2 = DHCPOFFER
    std::cerr << "DHCPOFFER error: mismatch option 53 value (" 
      << (int)it_53->second[0] << ")" << std::endl;
    return -1;
  }

  auto it_54 = offer_options.find(54);
  if (it_54 == offer_options.end() || it_54->second.size() != 4) {
    std::cerr << "DHCPOFFER error: invalid option 54" << std::endl;
    return -1;
  }
  std::vector<uint8_t> server_id_packed = it_54->second;
  uint32_t server_id_unpacked;
  offer_packet.unpack_ip(it_54->second, server_id_unpacked);
  if (server_id_unpacked != this->inbound_packet.sin_addr.s_addr) {
    std::cerr << "DHCPOFFER error: server id does not match packet source";
    return -1;
  }
  this->change_server_id(server_id_unpacked);

  uint32_t lease_time, t1_renewal, t2_rebinding;
  auto it_51 = offer_options.find(51);
  if (it_51 == offer_options.end() || it_51->second.size() != 4) return -1;
  offer_packet.unpack_time(it_51->second, lease_time);
  if (lease_time == 0) {
    std::cerr << "DHCPOFFER error: invalid lease time 0" << std::endl;
    return -1;
  }

  auto it_58 = offer_options.find(58);
  if (it_58 == offer_options.end() || it_58->second.size() != 4) {
    t1_renewal = lease_time * 0.5; // RFC default if missing
  } else {
    offer_packet.unpack_time(it_58->second, t1_renewal);
  }

  auto it_59 = offer_options.find(59);
  if (it_59 == offer_options.end() || it_59->second.size() != 4) {
    t2_rebinding = lease_time * 0.875; // RFC default if missing
  } else {
    offer_packet.unpack_time(it_59->second, t2_rebinding);
  }

  if (!(t1_renewal < t2_rebinding && t2_rebinding < lease_time)) {
    std::cerr << "DHCPOFFER error: invalid timer hierarchy" << std::endl;
    return -1;
  }

  this->change_lease_time(lease_time);
  this->change_renewal_time(t1_renewal);
  this->change_rebind_time(t2_rebinding);

  auto it_1 = offer_options.find(1);
  if (it_1 == offer_options.end()) {
    std::cerr << "DHCPOFFER error: cannot find option 1 for netmask"; 
    return -1;
  }
  std::vector<uint8_t> netmask_packed = it_1->second;
  uint32_t netmask_unpacked;
  offer_packet.unpack_ip(netmask_packed, netmask_unpacked);
  this->change_netmask(netmask_unpacked);

  return 0;
}

int dhcp_client_context::validate_acknowledge(dhcp_packet& acknowledge_packet) {
  // message type
  auto acknowledge_options = acknowledge_packet.options; 

  auto it_53 = acknowledge_options.find(53);
  if (it_53 == acknowledge_options.end()) {
    std::cerr << "DHCPACKNOWLEDGE error: cannot find option 53";
    return -1;
  }
  const uint8_t MESSAGE_TYPE = it_53->second[0];
  if (MESSAGE_TYPE != 5) {
    std::cerr << "DHCPACKNOWLEDGE error: invalid message type (" << MESSAGE_TYPE << ")";
    return -1;
  }

  // verify server id
  auto it_54 = acknowledge_options.find(54);
  if (it_54 == acknowledge_options.end()) {
    std::cerr << "DHCPACKNOWLEDGE error: cannot find option 54";
    return -1;
  }
  std::vector<uint8_t> server_id_packed = it_54->second;
  uint32_t server_id_unpacked;
  acknowledge_packet.unpack_ip(server_id_packed, server_id);
  if (server_id_unpacked != this->server_id) {
    std::cerr << "DHCPACKNOWLEDGE error: server id mismatch, expected " << this->server_id << ", got " << server_id_unpacked;
    return -1;
  }
  // verify timer
  auto it_51 = acknowledge_options.find(51);
  if (it_51 == acknowledge_options.end()) {
    std::cerr << "DHCPACKNOWLEDGE error: cannot find option 51";
    return -1;
  }
  std::vector<uint8_t> lease_time_packed = it_51->second;
  uint32_t lease_time_unpacked;
  acknowledge_packet.unpack_time(lease_time_packed, lease_time_unpacked);
  if (lease_time_unpacked != this->lease_time) {
    std::cerr << "DHCPACKNOWLEDGE error: lease time mismatch";
    return -1;
  }

  // check XID
  const uint32_t inbound_xid = acknowledge_packet.header.xid;
  if (this->xid != inbound_xid) {
    std::cerr << "DHCPACKNOWLEDGE error: xid mismatch (expected "
              << this->xid << ", got " << inbound_xid;
    return -1;
  }
  // hardware address
  const uint8_t* client_mac = this->mac.data();
  const uint8_t* inbound_mac = acknowledge_packet.header.chaddr;
  if ( std::memcmp(client_mac, inbound_mac, 6) != 0) {
    std::cerr << "DHCPACKNOWLEDGE error: inbound mac does not match client mac";
    return -1;
  }
  // check yiaddr from acknowledge
  const uint32_t inbound_yiaddr = acknowledge_packet.header.yiaddr;
  if (this->yiaddr != inbound_yiaddr) {
    std::cerr << "DHCPACKNOWLEDGE error: offered ip does not match acknowledged ip";
    return -1;
  }

  // timer validation
  return 0;
}

void dhcp_client_context::perform_broadcast(dhcp_packet discovery_packet) {
  sockaddr_in dest_addr;
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_addr.s_addr = INADDR_BROADCAST;
  dest_addr.sin_port = htons(DUMMYHCP_PORT);

  socklen_t addr_len = sizeof(dest_addr);

  discovery_packet.preflight_order_change();
  ssize_t bytes_serialized = discovery_packet.serialize(this->packet_buf, sizeof(discovery_packet));
  size_t bytes_sent = sendto(this->sockfd, packet_buf, bytes_serialized, 0, (struct sockaddr*)&dest_addr, addr_len);
  if (bytes_sent < 0) {
    std::cerr << "Broadcast error: failed to send bytes";
  }
  return;
}

void dhcp_client_context::perform_request(dhcp_packet request_packet) {
  // build appropriate header (DHCPREQUEST)
  this->build_client_header(request_packet);
  request_packet.add_option(53);
  request_packet.options[53].push_back(3);

  request_packet.add_option(54);
  std::vector<uint8_t> packed_server_id;
  request_packet.pack_ip(packed_server_id, this->server_id);
  for (auto x : packed_server_id) {
    request_packet.options[54].push_back(x); 
  }

  // set the appropriate options
  request_packet.add_option(50);
  std::vector<uint8_t> packed_ip;
  request_packet.pack_ip(packed_ip, this->yiaddr);
  for (auto x : packed_ip) {
    request_packet.options[50].push_back(x); 
  }

  // set the appropriate options
  request_packet.add_option(55);
  std::vector<uint8_t> request_list = {1, 3, 6};
  for (auto x : request_list) {
    request_packet.options[55].push_back(x); 
  }

  sockaddr_in dest_addr;
  std::memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(DUMMYHCP_PORT);
  dest_addr.sin_addr.s_addr = INADDR_BROADCAST; 
  socklen_t addr_len = sizeof(dest_addr);

  ssize_t bytes_serialized = request_packet.serialize(this->packet_buf, sizeof(request_packet));
  ssize_t bytes_sent = sendto(this->sockfd, this->packet_buf, bytes_serialized, 0, (const sockaddr*)&dest_addr, addr_len);
  if (bytes_sent < 0) {
    std::cerr << "Broadcast error: cannot send DHCPREQUEST";
  }

  return;
}

/*
void dhcp_client_context::unpack_offer(dhcp_packet offer_packet) {
  auto OPTIONS = offer_packet.options;

  std::vector<uint8_t> server_id_unpacked = OPTIONS[54];
  std::vector<uint8_t> lease_time_unpacked = OPTIONS[51];
  std::vector<uint8_t> renewal_time_unpacked = OPTIONS[58];
  std::vector<uint8_t> rebind_time_unpacked = OPTIONS[59];
  std::vector<uint8_t> requested_options = OPTIONS[55];

  uint32_t server_id;
  uint32_t lease_time;
  uint32_t renewal_time; 
  uint32_t rebind_time;

  offer_packet.unpack_ip(server_id_unpacked, server_id);
  offer_packet.unpack_time(lease_time_unpacked, lease_time);
  offer_packet.unpack_time(renewal_time_unpacked, renewal_time);
  offer_packet.unpack_time(rebind_time_unpacked, rebind_time);

  this->server_id = server_id;
  this->lease_time = lease_time;
  this->renewal_time = renewal_time;
  this->rebind_time = rebind_time;
  this->requested_options = options;

  this->yiaddr = offer_packet.header.yiaddr;
  this->siaddr = offer_packet.header.siaddr;
  this->giaddr = offer_packet.header.giaddr;

  return;
}
*/

void dhcp_client_context::bind_ip() {
  return;
}
