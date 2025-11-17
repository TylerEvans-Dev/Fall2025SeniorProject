#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(9000);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("Waiting for connection...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected!\n");

    while (1) {
        int n = recv(client_fd, buffer, sizeof(buffer)-1, 0);
        if (n <= 0) break;  // disconnect or error
        buffer[n] = 0;
        printf("Received: %s\n", buffer);
    }

    close(client_fd);
    close(server_fd);
}
