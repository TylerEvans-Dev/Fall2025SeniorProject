#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define VL53L0X_I2C_ADDR 0x29

static int fd = -1;

static void i2c_write_reg(uint8_t reg, uint8_t val) {
    uint8_t buf[2] = { reg, val };
    write(fd, buf, 2);
}

static void i2c_write_multi(uint8_t reg, uint8_t *data, uint8_t len) {
    uint8_t buf[32];
    buf[0] = reg;
    for (int i = 0; i < len; i++) buf[i + 1] = data[i];
    write(fd, buf, len + 1);
}

static void i2c_read_multi(uint8_t reg, uint8_t *data, uint8_t len) {
    write(fd, &reg, 1);
    read(fd, data, len);
}

int vl53l0x_init(const char *i2c_dev) {
    fd = open(i2c_dev, O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, VL53L0X_I2C_ADDR) < 0) {
        perror("ioctl");
        return -1;
    }

    // Boot sequence
    i2c_write_reg(0x88, 0x00);
    i2c_write_reg(0x80, 0x01);
    i2c_write_reg(0xFF, 0x01);
    i2c_write_reg(0x00, 0x00);
    i2c_write_reg(0x91, 0x3c);
    i2c_write_reg(0x00, 0x01);
    i2c_write_reg(0xFF, 0x00);
    i2c_write_reg(0x80, 0x00);

    // Increase timing budget for longer range (~50ms)
    uint8_t budget[] = { 0x00, 0x09, 0xC4 }; // 2500 * 20us = 50ms
    i2c_write_multi(0x01, budget, 3);

    usleep(200000);
    return 0;
}

uint16_t vl53l0x_read_mm(void) {
    uint8_t data[2];

    // Start single measurement
    i2c_write_reg(0x00, 0x01);
    usleep(50000); // wait ~50ms

    i2c_read_multi(0x14, data, 2);
    return ((uint16_t)data[0] << 8) | data[1];
}

void vl53l0x_close(void) {
    if (fd >= 0) close(fd);
}
