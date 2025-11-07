// Minimal VL53L0X distance reader for Linux I2C (Orange Pi, Raspberry Pi, etc.)
// Tested with /dev/i2c-3, returns valid distance in millimeters or -1 if invalid.
// Tyler-ready edition (fixes "256 mm" issue)

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define VL53L0X_ADDR 0x29

static int i2c_fd = -1;

static void write_reg(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    write(i2c_fd, buf, 2);
}

static void write_multi(uint8_t reg, uint8_t *data, uint8_t len) {
    uint8_t buf[32];
    buf[0] = reg;
    for (int i = 0; i < len; i++) buf[i+1] = data[i];
    write(i2c_fd, buf, len + 1);
}

static void read_multi(uint8_t reg, uint8_t *data, uint8_t len) {
    write(i2c_fd, &reg, 1);
    read(i2c_fd, data, len);
}

static void perform_ref_calibration(uint8_t vhv_init_byte) {
    write_reg(0x00, 0x01); // system fresh
    write_reg(0x80, 0x01);
    write_reg(0xFF, 0x01);
    write_reg(0x00, 0x00);
    write_reg(0x91, vhv_init_byte);
    write_reg(0x00, 0x01);
    write_reg(0xFF, 0x00);
    write_reg(0x80, 0x00);
}

// Wait for measurement ready
static int wait_measure_ready(void) {
    uint8_t status = 0;
    for (int i = 0; i < 50; i++) { // ~50 * 10ms = 0.5s timeout
        read_multi(0x13, &status, 1);
        if (status & 0x07) return 0; // bits[2:0] != 0 => ready
        usleep(10000);
    }
    return -1;
}

// Start a single measurement and return distance in mm or -1 if invalid
static int get_distance_mm(void) {
    write_reg(0x00, 0x01); // SYSRANGE_START = 1

    if (wait_measure_ready() < 0)
        return -1;

    uint8_t buf[12];
    read_multi(0x13, buf, 12);

    uint8_t range_status = buf[1];
    uint16_t distance = (buf[11] << 8) | buf[10];

    // Clear interrupts
    write_reg(0x0B, 0x01);

    // Validate reading
    if (distance == 0x1FFF || range_status != 0) {
        return -1; // invalid or sigma fail
    }

    return distance;
}

int main(void) {
    const char *dev = "/dev/i2c-3";
    if ((i2c_fd = open(dev, O_RDWR)) < 0) {
        perror("open");
        return 1;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, VL53L0X_ADDR) < 0) {
        perror("ioctl");
        return 1;
    }

    printf("VL53L0X minimal driver started on %s (0x29)\n", dev);

    // Basic setup: boot and ref calibration
    usleep(100000); // wait 100 ms after power-up
    perform_ref_calibration(0x40); // VHV
    perform_ref_calibration(0x00); // Phase

    while (1) {
        int dist = get_distance_mm();
        if (dist < 0)
            printf("Out of range or invalid\n");
        else
            printf("Distance = %d mm\n", dist);
        usleep(200000); // 5 Hz updates
    }

    return 0;
}
