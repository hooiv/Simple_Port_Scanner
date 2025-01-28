#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>

void port_scanner(char *ip, char *first_port_str, char *last_port_str);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <ip_address> <start_port> <end_port>\n", argv[0]);
        printf("Example: %s 127.0.0.1 1 100\n", argv[0]);
        return 1;
    }

    char ip[16] = {0};
    strcpy(ip, argv[1]);
    char first_port_str[6] = {0};
    strcpy(first_port_str, argv[2]);
    char last_port_str[6] = {0};
    strcpy(last_port_str, argv[3]);

    port_scanner(ip, first_port_str, last_port_str);
    return 0;
}

void port_scanner(char *ip, char *first_port_str, char *last_port_str) {
    int start_port = atoi(first_port_str);
    int end_port = atoi(last_port_str);

    if (start_port <= 0 || end_port <= 0 || start_port > 65535 || end_port > 65535 || start_port > end_port) {
        fprintf(stderr, "Invalid port range.\n");
        return;
    }

    printf("Scanning ports on %s from %d to %d...\n", ip, start_port, end_port);

    for (int port = start_port; port <= end_port; port++) {
        int sockfd = 0;
        struct sockaddr_in server_addr;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Socket creation failed");
            continue; // Continue to the next port even if socket creation fails
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port); // Convert port to network byte order
        if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
            fprintf(stderr, "Invalid address: %s\n", ip);
            close(sockfd);
            continue; // Continue to the next port if IP address is invalid
        }

        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
            printf("Port %d: Open\n", port);
            close(sockfd); // Close the socket after successful connection
        } else {
            // Port is likely closed or filtered. We don't print error for every closed port to keep output clean.
            // If you want to see errors for closed ports, uncomment the perror line.
            // perror("Connect failed");
            close(sockfd); // Close the socket even if connection fails
        }
    }

    printf("Port scanning finished.\n");
}