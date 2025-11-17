#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "cleanup.c"
#include "GY521.h"

int main() {
    //this is a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    inet_pton(AF_INET, "10.0.0.67", &server.sin_addr);

    connect(sock, (struct sockaddr *)&server, sizeof(server));
    //this is an example on how to use the accl.
    int fd;
    int q = wiringPiSetup();
    DataAccel d;
    int check = 0;
    if (q < 0){
        printf("something has gone wrong with orange setup\n");
        return -1;
    }
    int suc = initGY521("/dev/i2c-4", &fd, &check);
    while (1) {
        const char *msg = "Hello world\n";
        send(sock, msg, strlen(msg), 0);
        int readsuc = mpu_read_all(&fd, &d);
        printf("A: %.2f %.2f %.2f | G: %.2f %.2f %.2f | T: %.2f°C\n",
                       d.ax, d.ay, d.az, d.gx, d.gy, d.gz, d.temp);

        send(sock, &d, sizeof(d), 0);
        usleep(100000); // 100ms between sends
    }

    close(sock);
}
