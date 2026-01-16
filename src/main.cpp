#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "dhcp_packet.h"
#include "server.h"

// Standard DHCP ports require root privileges
#define MAXLINE 1500
#define PORT 8080

int main(int argc, char** argv) {
    const char* ARG_SERVER = "server";
    const char* ARG_CLIENT = "client";

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [server|client]" << std::endl;
        return EXIT_FAILURE;
    }

    // Comparison buffer for argument parsing
    char arg_buf[64];
    std::memset(arg_buf, 0, sizeof(arg_buf));
    std::strncpy(arg_buf, argv[1], sizeof(arg_buf) - 1);

    if (std::strncmp(arg_buf, ARG_SERVER, strlen(ARG_SERVER)) == 0) {
        // --- SERVER MODE ---
        std::cout << "Entering server mode on port " << PORT << "..." << std::endl;

        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket failed");
            return EXIT_FAILURE;
        }

        sockaddr_in servaddr, cliaddr;
        std::memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
            perror("bind failed");
            close(sockfd);
            return EXIT_FAILURE;
        }

        std::cout << "Waiting for DHCP packets..." << std::endl;
        uint8_t recv_buf[MAXLINE];
        socklen_t len = sizeof(cliaddr);

        ssize_t n = recvfrom(sockfd, recv_buf, MAXLINE, 0, (struct sockaddr*)&cliaddr, &len);
        if (n >= 0) {
            std::cout << "Received " << n << " bytes from " << inet_ntoa(cliaddr.sin_addr) << std::endl;
            // Next step: dhcp_packet.packet_from_stream(recv_buf, n);
        }

        dhcp_packet dp = dhcp_packet();
        dp.packet_from_stream(recv_buf, n);
        dp.print();

        close(sockfd);
    }
    else if (std::strncmp(arg_buf, ARG_CLIENT, strlen(ARG_CLIENT)) == 0) {
        // --- CLIENT MODE ---
        std::cout << "Entering client mode (broadcasting)..." << std::endl;

        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket failed");
            return EXIT_FAILURE;
        }

        int broadcastEnable = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
            perror("setsockopt failed");
            close(sockfd);
            return EXIT_FAILURE;
        }

        sockaddr_in dest_addr;
        std::memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        dest_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

        // Prepare DHCP Payload
        uint8_t fake_mac[6] = {0x69, 0x69, 0x69, 0x69, 0x69, 0x69};
        dhcp_packet dp;

        // Note: inet_addr already returns Network Byte Order
        uint32_t zero_ip = inet_addr("0.0.0.0");
        uint32_t relay_ip = inet_addr("192.168.100.1");

        dp.build_client_header(zero_ip, relay_ip, generate_xid(), fake_mac);

        // Option 53: DHCP Message Type (1 = Discover)
        dp.options[53].push_back(1);

        uint8_t packet_buffer[MAXLINE];
        std::memset(packet_buffer, 0, MAXLINE);
        ssize_t bytes_serialized = dp.serialize(packet_buffer, MAXLINE);

        if (bytes_serialized <= 0) {
            std::cerr << "Serialization failed!" << std::endl;
            close(sockfd);
            return EXIT_FAILURE;
        }

        // Send serialized binary data
        ssize_t bytes_sent = sendto(sockfd, packet_buffer, bytes_serialized, 0,
                                    (struct sockaddr*)&dest_addr, sizeof(dest_addr));

        if (bytes_sent < 0) {
            perror("sendto failed");
        } else {
            std::cout << "Broadcasted DHCP Discovery: " << bytes_sent << " bytes." << std::endl;
        }

        close(sockfd);
    } 
    else {
        std::cerr << "Invalid argument. Use 'server' or 'client'." << std::endl;
    }

    return 0;
}