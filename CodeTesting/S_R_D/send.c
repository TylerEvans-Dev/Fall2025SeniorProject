#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    inet_pton(AF_INET, "10.0.0.67", &server.sin_addr);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    while (1) {
        const char *msg = "Hello world\n";
        send(sock, msg, strlen(msg), 0);
        usleep(100000); // 100ms between sends
    }

    close(sock);
}
