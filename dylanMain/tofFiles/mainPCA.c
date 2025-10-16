//
// Time of Flight sensor test program
//
// Copyright (c) 2017 Larry Bank
// email: bitbank@pobox.com
// Project started 7/29/2017
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <tof.h> // time of flight sensor library

#define I2C_BUS "/dev/i2c-4"
#define TCA_ADDR 0x70
#define TOF_ADDR 0x29

uint16_t* read_PCA_channels(void) {
	static uint16_t distances[8];
	int fd = open(I2C_BUS, O_RDWR);
	for (int ch = 0; ch < 8; ch++) {
		ioctl(fd, I2C_SLAVE, TCA_ADDR);
		uint8_t config = 1 << ch;
		write(fd, &config, 1);
		ioctl(fd, I2C_SLAVE, TOF_ADDR);
		tofInit(4, TOF_ADDR, 1);
		distances[ch] = tofReadDistance();
	}
	close(fd);
	return distances;
}




int main() {
	uint16_t *d = read_PCA_channels();
	for (int ch=0; ch <8; ch++) {
		printf("Channel %d: Distance = %u mm\n", ch, d[ch]);
	}
	return 0;
}
