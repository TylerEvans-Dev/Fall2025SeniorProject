#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    while(1){
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    int broadcastEnable = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = inet_addr("192.168.1.255");  // broadcast address

    char msg[] = "DISCOVER_REQUEST";
    sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&addr, sizeof(addr));

    printf("Broadcast message sent\n");

    close(sock);
    }
    return 0;
}
