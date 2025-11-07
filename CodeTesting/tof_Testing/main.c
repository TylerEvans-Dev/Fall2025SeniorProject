#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "vl5301x.c"

int vl53l0x_init(const char *dev);
uint16_t vl53l0x_read_mm(void);
void vl53l0x_close(void);

int main(void) {
    if (vl53l0x_init("/dev/i2c-3") < 0) {
        printf("Failed to init VL53L0X\n");
        return 1;
    }

    printf("VL53L0X ready. Reading...\n");
    while (1) {
        uint16_t d = vl53l0x_read_mm();
        printf("Distance = %d mm\n", d);
        usleep(200000);
    }

    vl53l0x_close();
    return 0;
}
