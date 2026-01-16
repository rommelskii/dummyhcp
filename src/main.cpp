//
// Created by MJ Ronduen on 1/16/26.
//

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


// ./dhcp (server | client)

#define MAXLINE 1500
#define PORT 8080

int main(int argc, char** argv) {
    const char* ARG_SERVER = "server";
    const char* ARG_CLIENT = "client";
    if (argc < 2) {
        std::cerr << "Error: incorrect parameters";
        exit(EXIT_FAILURE);
    }
    char buf[MAXLINE];
    std::memcpy(buf, argv[1], MAXLINE);

    if ( strncmp(buf, ARG_SERVER, MAXLINE) == 0 ) {
        std::cout << "Entering server mode\n";
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1) {
            perror("socket failed");
        }
        sockaddr_in servaddr, cliaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        if ( bind(sockfd, (const struct sockaddr*) &servaddr, sizeof(servaddr)) < 0 ) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr*) &cliaddr, &len);
        buf[n] = '\0';
        std::cout << "Received: " << buf << std::endl;
    }
    else if ( strncmp(buf, ARG_CLIENT, MAXLINE) == 0 ) {
        std::cout << "Entering client mode\n";
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        int broadcastEnable = 1;
        int ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcastEnable, sizeof(broadcastEnable));
        if (ret < 0) {
            perror("setsockopt failed");
            exit(EXIT_FAILURE);
        }

        sockaddr_in servaddr, cliaddr;

        std::memset(&servaddr, 0, sizeof(servaddr));
        std::memset(&cliaddr, 0, sizeof(cliaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

        ssize_t bytes_sent = sendto(sockfd, buf, MAXLINE, 0, (struct sockaddr*) &servaddr, sizeof(servaddr));

        close(sockfd);
    }
    return 0;
}
