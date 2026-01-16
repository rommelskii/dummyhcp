#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>  // REQUIRED for memset, memcpy, strncmp
#include "dhcp_packet.h"
#include "server.h"

#define MAXLINE 1500
#define PORT 8080

int main(int argc, char** argv) {
    const char* ARG_SERVER = "server";
    const char* ARG_CLIENT = "client";

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [server|client]" << std::endl;
        return EXIT_FAILURE;
    }

    // Safely copy the argument to our buffer
    char buf[MAXLINE];
    std::memset(buf, 0, MAXLINE);
    std::strncpy(buf, argv[1], MAXLINE - 1);

    if (std::strncmp(buf, ARG_SERVER, strlen(ARG_SERVER)) == 0) {
        std::cout << "Entering server mode on port " << PORT << "..." << std::endl;
        
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket failed");
            return EXIT_FAILURE;
        }

        sockaddr_in servaddr, cliaddr;
        std::memset(&servaddr, 0, sizeof(servaddr));
        std::memset(&cliaddr, 0, sizeof(cliaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
            perror("bind failed");
            close(sockfd);
            return EXIT_FAILURE;
        }

        std::cout << "Waiting for data..." << std::endl;
        socklen_t len = sizeof(cliaddr);
        // Receive MAXLINE-1 to leave room for the null terminator
        ssize_t n = recvfrom(sockfd, buf, MAXLINE - 1, 0, (struct sockaddr*)&cliaddr, &len);
        
        if (n >= 0) {
            buf[n] = '\0';
            std::cout << "Received from " << inet_ntoa(cliaddr.sin_addr) << ": " << buf << std::endl;
        }
        
        close(sockfd);
    } 
    else if (std::strncmp(buf, ARG_CLIENT, strlen(ARG_CLIENT)) == 0) {
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

        sockaddr_in servaddr;
        std::memset(&servaddr, 0, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

        // begin payload
        uint8_t fake_mac[8] = {0x69, 0x69, 0x69, 0x69, 0x69, 0x69};
        dhcp_packet dp;
        dp.build_client_header(inet_addr("0.0.0.0"), inet_addr("192.168.100.1"), generate_xid(), fake_mac);
        dp.options[53].push_back(1);
        dp.options[53].push_back(1);

        uint8_t buf[MAXSIZE];
        ssize_t bytes_serialized = dp.serialize(buf, MAXSIZE);

        ssize_t bytes_sent = sendto(sockfd, (const char*)buf, bytes_serialized, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));

        if (bytes_sent < 0) {
            perror("sendto failed");
        } else {
            std::cout << "Broadcasted " << bytes_sent << " bytes." << std::endl;
        }

        close(sockfd);
    } 
    else {
        std::cerr << "Invalid argument. Use 'server' or 'client'." << std::endl;
    }

    return 0;
}