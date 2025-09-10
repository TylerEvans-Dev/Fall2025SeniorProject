#include <stdio.h>
#include <unistd.h>
//There will be several things underlined as they are not fixed at the moment :(
#include "test.c"
#include "gpio.c"

#define CONSUMER "blink"

int main(void) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int ret;

    // Open gpiochip1 (most Orange Pi GPIOs are here)
    chip = gpiod_chip_open("/dev/gpiochip1");
    if (!chip) { perror("Open chip failed"); return 1; }

    // Pick GPIO line number (e.g. 264 for PI8)

    line = gpiod_chip_get_line(chip, PIN3_SDA1);
    if (!line) { perror("Get line failed"); gpiod_chip_close(chip); return 1; }

    // Request as output, default low
    ret = gpiod_line_request_output(line, CONSUMER, 0);
    if (ret < 0) { perror("Request line as output failed"); gpiod_chip_close(chip); return 1; }

    // Blink 10 times
    for (int i = 0; i < 10; i++) {
        gpiod_line_set_value(line, 1); // ON
        sleep(1);
        gpiod_line_set_value(line, 0); // OFF
        sleep(1);
    }

    gpiod_chip_close(chip);
    return 0;
}
