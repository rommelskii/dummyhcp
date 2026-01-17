#include <iostream>
#include <vector>
#include <unordered_map>

#include "packet.h"

const uint32_t MAGIC_COOKIE = 0x63825363;

void dhcp_packet::build_header(uint8_t op, uint32_t xid, uint16_t secs, uint16_t flags,
                  uint32_t ciaddr, uint32_t yiaddr, uint32_t siaddr,
                  uint32_t giaddr, uint8_t chaddr[16]) {
  this->header.op = op;
  this->header.htype = 1;
  this->header.hlen = 6;
  this->header.hops = 0;
  this->header.xid = xid;
  this->header.secs = secs;
  this->header.flags = flags;
  this->header.ciaddr = ciaddr;
  this->header.yiaddr = yiaddr;
  this->header.siaddr = siaddr;
  this->header.giaddr = giaddr;

  std::memcpy(chaddr, this->header.chaddr, (size_t)sizeof(this->header.chaddr));
  std::memset(this->header.sname, 0, sizeof(this->header.sname));
  std::memset(this->header.file, 0, sizeof(this->header.file));

  this->header.magic_cookie = MAGIC_COOKIE;
}

void dhcp_packet::preflight_order_change() {
  dhcp_header temp_dh = this->header;
  temp_dh.xid = htonl(temp_dh.xid);
  temp_dh.secs = htonl(temp_dh.secs);
  temp_dh.flags = htonl(temp_dh.flags);
  temp_dh.ciaddr = htonl(temp_dh.ciaddr);
  temp_dh.yiaddr = htonl(temp_dh.yiaddr);
  temp_dh.siaddr = htonl(temp_dh.siaddr);
  temp_dh.giaddr = htonl(temp_dh.giaddr);
  temp_dh.magic_cookie = htonl(temp_dh.magic_cookie);
  this->header = temp_dh;
}

ssize_t dhcp_packet::serialize(uint8_t buf[], size_t max_len) {
  this->preflight_order_change(); //change appropriate packet fields to network order
  // begin copying header to byte buffer
  std::memcpy(&this->header, buf, sizeof(this->header));
  size_t offset = sizeof(this->header); // create pointer after header
  uint32_t magic_cookie = this->header.magic_cookie;
  std::memcpy(buf+offset, &magic_cookie, sizeof(magic_cookie));
  for (const auto& [type, value] : this->options) {
    if (offset + 2 + value.size() > (size_t)max_len) return -1; // boundary check if payload is within length specified
    buf[offset++] = type;
    buf[offset++] = (uint8_t)value.size();
    std::memcpy(buf + offset, value.data(), value.size()); // copy the variable-sized option value field to the buffer
    offset += value.size();
  }
  if (offset < (size_t)max_len) {
    buf[offset++] = 0xFF;
  } else {
    return -1;
  }
  return (ssize_t)offset;
}

ssize_t dhcp_packet::deserialize(uint8_t buf[], size_t bytes_received) {
  if (bytes_received < sizeof(this->header)) {
    std::cerr << "error: header size mismatch";
    return -1;
  }

  std::memcpy(buf, &(this->header), sizeof(this->header)); // extract header from bytes to header

  uint8_t* options_start = buf + sizeof(this->header); // pointer where options field starts
  uint8_t* options_end = buf + bytes_received;
  uint8_t* current = options_start;
  while (current < options_end) {
    if (*current == 255) break; // stop if 0xFF (end of options)
    if (*current == 0) { // skip 0 padding
      current++;
      continue;
    } 

    uint8_t value_length = *(current + 1);
    if ((current + 2 + value_length) > options_end) {
      std::cerr << "error: options length mismatch";
      break;
    }
    for (int i=0; i<value_length; ++i) {
      this->options[*current].push_back(*current + 2 + i);
    }

    current += (2 + value_length);
  }

  if (current != options_end) {
    return -1;
  }

  return bytes_received;
}
