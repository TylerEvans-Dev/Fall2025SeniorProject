#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#if defined(__APPLE__)
#include "wiringOP/wiringPi/wiringPi.h"
#include "wiringOP/wiringPi/wiringPiI2C.h"
#else
#include <wiringPi.h>
#include <wiringPiI2C.h>
#endif

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

int main(void) {
    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, 0x29) < 0) {
        perror("ioctl");
        return -1;
    }

    uint8_t reg = 0xC0;
    write(fd, &reg, 1);
    uint8_t val;
    read(fd, &val, 1);
    printf("Register 0xC0 = 0x%02X\n", val);
    close(fd);
    return 0;
}





}
