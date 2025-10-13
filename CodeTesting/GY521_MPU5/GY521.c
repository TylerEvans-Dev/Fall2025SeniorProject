#include "GY521.h"
#include <stdint.h>

#define ADDR 0x68
#define PW_MANG 0x6B
#define ACCEL_XOUT_H 0x3B
#define ACCEL_SCALE 16384.0
#define GYRO_SCALE 131.0
#define TEMP_SCALE 340.0
#define TEMP_FACTOR 36.53

#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C


int initGY521(const char *chan, int *fd, int *check){
    *fd = wiringPiI2CSetupInterface(chan, ADDR);
    if(fd < 0)
    {
        return -1;
    }
    uint8_t res = wiringPiI2CReadReg8(*fd, 0x75);
    if(res != ADDR){
        printf("The device is not found please try again fool\n");
        printf("the device read %d or %n \n", res, res);
    }

    printf("success the device is connected at %d and is 0x68 \n", res);
    //sensor needs some warm up time :)
    uint8_t res2 = wiringPiI2CWriteReg8(*fd, PW_MANG, 0x00);
    delay(10);
    if(res2 < 0){
        printf("error the sensor did not write to the device\n");
        return -1;
    }
    printf("the sensor has been booted up %d \n", res2);
    //this is for error checking
    // int i;
    // printf("WHO_AM_I: 0x%02X\n", wiringPiI2CReadReg8(*fd, 0x75));
    // printf("PWR_MGMT_1: 0x%02X\n", wiringPiI2CReadReg8(*fd, 0x6B));
    // printf("Dump 0x3B..0x48:\n");
    // for (i = 0x3B; i <= 0x48; ++i) {
    //     int v = wiringPiI2CReadReg8(*fd, i);
    //     if (v < 0) printf("reg 0x%02X: read error (%d)\n", i, v);
    //     else printf("reg 0x%02X: 0x%02X (%d)\n", i, v, v);
    // }
    *check = 1;
    return 0;
}

int mpu6050_read_all(int *fd, DataAccel *data) {
    uint8_t buf[14];
    for (int i = 0; i < 14; i++){
        buf[i] = wiringPiI2CReadReg8(*fd, ACCEL_XOUT_H + i);
    }
    //This is putting the data in both the higher and lower parts of the 16 bit number;
    int16_t ax = (buf[0] << 8) | buf[1];
    int16_t ay = (buf[2] << 8) | buf[3];
    int16_t az = (buf[4] << 8) | buf[5];
    int16_t temp = (buf[6] << 8) | buf[7];
    int16_t gx = (buf[8] << 8) | buf[9];
    int16_t gy = (buf[10] << 8) | buf[11];
    int16_t gz = (buf[12] << 8) | buf[13];

    data->ax = ax / ACCEL_SCALE;
    data->ay = ay / ACCEL_SCALE;
    data->az = az / ACCEL_SCALE;
    data->gx = gx / GYRO_SCALE;
    data->gy = gy / GYRO_SCALE;
    data->gz = gz / GYRO_SCALE;
    data->temp = (temp / TEMP_SCALE) + TEMP_FACTOR;

    return 0;
}
float readXAce(int *fd, int check){
    if(check > 0){
        uint8_t hb = wiringPiI2CReadReg8(*fd, 0x3B);
        uint8_t lb = wiringPiI2CReadReg8(*fd, 0X3C);
        return ((hb << 8) | lb) / ACCEL_SCALE;
    }
    else{
        return -1;
    }
}
float readYAce(int *fd, int check){
    if(check > 0){
        uint8_t hb = wiringPiI2CReadReg8(*fd, 0x3D);
        uint8_t lb = wiringPiI2CReadReg8(*fd, 0X3E);
        return ((hb << 8) | lb) / ACCEL_SCALE;
    }
    else{
        return -1;
    }
}
float readZAce(int *fd, int check){
    if(check > 0){
        uint8_t hb = wiringPiI2CReadReg8(*fd, 0x3F);
        uint8_t lb = wiringPiI2CReadReg8(*fd, 0X40);
        return ((hb << 8) | lb) / ACCEL_SCALE;
    }
    else{
        return -1;
    }
}
float readXGyr(int *fd, int check){
    if(check > 0){
        uint8_t hb = wiringPiI2CReadReg8(*fd, 0x43);
        uint8_t lb = wiringPiI2CReadReg8(*fd, 0X44);
        return ((hb << 8) | lb) / GYRO_SCALE;
    }
    else{
        return -1;
    }
}
float readYGyr(int *fd, int check){
    if(check > 0){
        uint8_t hb = wiringPiI2CReadReg8(*fd, 0x45);
        uint8_t lb = wiringPiI2CReadReg8(*fd, 0X46);
        return ((hb << 8) | lb) / GYRO_SCALE;
    }
    else{
        return -1;
    }
}
float readZGyr(int *fd, int check){
    if(check > 0){
        uint8_t hb = wiringPiI2CReadReg8(*fd, 0x47);
        uint8_t lb = wiringPiI2CReadReg8(*fd, 0X48);
        return ((hb << 8) | lb) / GYRO_SCALE;
    }
    else{
        return -1;
    }
}
float readTemp(int *fd, int check){
    if(check > 0){
        uint8_t hb = wiringPiI2CReadReg8(*fd, 0x41);
        uint8_t lb = wiringPiI2CReadReg8(*fd, 0X42);
        return ((((hb << 8) | lb)) / TEMP_SCALE) + TEMP_FACTOR;
    }
    else{
        return -1;
    }
}
