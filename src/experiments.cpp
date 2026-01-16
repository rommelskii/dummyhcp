#include <iomanip>
#include <iostream>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dhcp_packet.h"


/*
int main() {
    char buf[1500];
    memset(buf, 0, sizeof(buf));
    uint8_t fake_mac[6] = {0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E};
    uint32_t fake_xid = generate_xid();
    uint32_t fake_ip = inet_addr("192.168.100.10");
    uint32_t fake_gateway = inet_addr("192.168.100.1");
    dhcp_packet dp;
    dp.build_client_header(fake_ip, fake_gateway, fake_xid, fake_mac);

    if (dp.header.op == 1) {
        std::cout << "Operation: BOOTREQUEST\n";
    }

    std::string mac = mac_to_string(dp.header.chaddr);
    std::cout << "Client MAC: "
              << mac << "\n";

    std::cout << "Transaction ID: 0x"
              << std::hex
              << std::setw(8)
              << std::setfill('0')
              << dp.header.xid
              << std::dec
              << std::endl;

    std::cout << "Client IP: "
              << inet_ntop(AF_INET, &dp.header.ciaddr, buf, sizeof(buf))
              << std::endl;

    std::cout << "Gateway: "
              << inet_ntop(AF_INET, &dp.header.giaddr, buf, sizeof(buf))
              << std::endl;

    return 0;
}

/*
int main() {
    int sockfd;
    uint8_t buf[MAXSIZE];
    sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);

    if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    socklen_t len = sizeof(cliaddr);
    int n;
    n = recvfrom(sockfd, buf, MAXSIZE, MSG_WAITALL, (struct sockaddr*)&cliaddr, &len);

    dhcp_packet packet;
    packet.packet_from_stream(buf, n);

    exit(EXIT_SUCCESS);
}
*/