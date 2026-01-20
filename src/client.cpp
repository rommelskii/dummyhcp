#include <iostream>
#include <gtest/gtest.h>
#include "unistd.h"

void dhcp_client::run_client() {
  //socket binding logic
  //all socket variables go here
  const int DUMMYHCP_PORT = 8069;
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
  int broadcast_enable = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
    perror("setsockopt failed");
    return;
  }

  sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_BROADCAST;
  servaddr.sin_port = htons(DUMMYHCP_PORT);

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
    perror("bind failed");
    return;
  }

  //begin business logic
  while (true) {
    dhcp_state current_state = this->get_state();
    switch (current_state) {
      case dhcp_state::INIT:
        break;
      case dhcp_state::SELECTING:
        break;
      case dhcp_state::REQUESTING:
        break;
      case dhcp_state::BOUND:
        break;
      case dhcp_state::RENEWING:
        break;
    }
  }

  close(sockfd);
}

