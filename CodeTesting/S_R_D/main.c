#include <stdint.h>
#include <stdio.h>
#include "GY521.h"
#include "cleanup.c"
int main(){
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
    while(1){
    //**** IMPORTANT *****
    //when using this sensor there is some dropout if read is done on their own so its good to read all
    // at once this is becuase there is typically an update that occurs and it needs
    //**** IMPORTANT ******
    int readsuc = mpu_read_all(&fd, &d);
    printf("A: %.2f %.2f %.2f | G: %.2f %.2f %.2f | T: %.2f°C\n",
                   d.ax, d.ay, d.az, d.gx, d.gy, d.gz, d.temp);
    printf("A x %.2f \n", readXAce(&fd, check));
    printf("A y %.2f \n", readYAce(&fd, check));
    printf("A z %.2f \n", readZAce(&fd, check));
    printf("G x %.2f \n", readXGyr(&fd, check));
    printf("G x %.2f \n", readYGyr(&fd, check));
    printf("G x %.2f \n", readZGyr(&fd, check));
    printf("T x %.2f \n", readTemp(&fd, check));

    delay(2000); // added the delay for creating the values
    }
    return 0;
}
