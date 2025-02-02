#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <errno.h>

#define MAX_THREADS 100  // Maximum number of concurrent threads
#define TIMEOUT 2         // Timeout for each connection in seconds

typedef struct {
    char ip[16];
    int port;
} PortScanArgs;

// Function to scan a single port
void *scan_port(void *arg) {
    PortScanArgs *args = (PortScanArgs *)arg;
    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        free(arg);
        return NULL;
    }

    // Set timeout for the connection attempt
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(args->port);
    inet_pton(AF_INET, args->ip, &server_addr.sin_addr);

    // Attempt to connect
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
        printf("Port %d: Open\n", args->port);
    }

    close(sockfd);
    free(arg);
    return NULL;
}

// Function to create and manage scanning threads
void port_scanner(char *ip, int start_port, int end_port) {
    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    printf("Scanning ports on %s from %d to %d...\n", ip, start_port, end_port);

    for (int port = start_port; port <= end_port; port++) {
        PortScanArgs *args = malloc(sizeof(PortScanArgs));
        strcpy(args->ip, ip);
        args->port = port;

        // Create a new thread for each port scan
        if (pthread_create(&threads[thread_count], NULL, scan_port, args) != 0) {
            perror("Thread creation failed");
            free(args);
        }

        thread_count++;

        // Manage thread pool: Wait for some to finish if limit reached
        if (thread_count >= MAX_THREADS) {
            for (int i = 0; i < thread_count; i++) {
                pthread_join(threads[i], NULL);
            }
            thread_count = 0;
        }
    }

    // Wait for any remaining threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Port scanning finished.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <ip_address> <start_port> <end_port>\n", argv[0]);
        return 1;
    }

    int start_port = atoi(argv[2]);
    int end_port = atoi(argv[3]);

    if (start_port <= 0 || end_port <= 0 || start_port > 65535 || end_port > 65535 || start_port > end_port) {
        fprintf(stderr, "Invalid port range.\n");
        return 1;
    }

    port_scanner(argv[1], start_port, end_port);
    return 0;
}
