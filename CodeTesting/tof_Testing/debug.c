// vl53_debug.c
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_DEV "/dev/i2c-3"
#define ADDR 0x29

int fd = -1;

int write_reg(uint8_t reg, uint8_t val) {
    uint8_t buf[2] = { reg, val };
    if (write(fd, buf, 2) != 2) return -1;
    return 0;
}

int read_regs(uint8_t reg, uint8_t *buf, int len) {
    if (write(fd, &reg, 1) != 1) return -1;
    if (read(fd, buf, len) != len) return -1;
    return 0;
}

int main(void) {
    fd = open(I2C_DEV, O_RDWR);
    if (fd < 0) { perror("open"); return 1; }
    if (ioctl(fd, I2C_SLAVE, ADDR) < 0) { perror("ioctl"); return 1; }

    // Trigger a single measurement (SYSRANGE_START = 0x00)
    write_reg(0x00, 0x01);

    // Wait for data ready: poll RESULT_INTERRUPT_STATUS (0x13, 1 byte)
    int timeout = 0;
    uint8_t v=0;
    while (timeout < 100) {
        if (read_regs(0x13, &v, 1) == 0) {
            // bit0..2 show interrupt status; non-zero when ready
            if ((v & 0x07) != 0) break;
        }
        usleep(5000);
        timeout++;
    }
    printf("poll cycles: %d, interrupt status reg(0x13) = 0x%02x\n", timeout, v);

    // Read RESULT_RANGE_STATUS (0x14) and the next 6 bytes (dump)
    uint8_t dump[10] = {0};
    if (read_regs(0x13, dump, 10) == 0) {
        printf("Regs 0x13..0x1C: ");
        for (int i=0;i<10;i++) printf("%02x ", dump[i]);
        printf("\n");
    } else {
        perror("read dump");
    }

    // Read final range mm - many libs read 0x1E/0x1F or RESULT_RANGE_STATUS+10
    uint8_t r[2] = {0};
    if (read_regs(0x1E, r, 2) == 0) {
        printf("Final range raw at 0x1E/0x1F: %02x %02x -> %u mm\n", r[0], r[1], (r[0]<<8)|r[1]);
    } else {
        perror("read final");
    }

    // Clear interrupt (write 0x01 to SYSTEM_INTERRUPT_CLEAR 0x0B)
    write_reg(0x0B, 0x01);

    close(fd);
    return 0;
}
