#include <stdio.h>
#include <unistd.h>
/*
 * [1] OpenAI, "How could I use libgpio on Mac?," ChatGPT, Sep. 1, 2025. [Online]. Available: https://chat.openai.com
 * Mock GPIO implementation for macOS (no real /dev/gpiochipN support).
 * Adapted from ChatGPT (OpenAI), response dated 2025-09-01.
 */
//this here is a macro of all the functions for lib gpio here is some doc.
#ifdef __APPLE__
#define gpiod_chip void
//this is the gpiod_chip function
#define gpiod_line void
//this is the gpiod line function
#define gpiod_chip_open(name) (NULL)
#define gpiod_chip_get_line(chip, line) (NULL)
#define gpiod_line_request_output(line, consumer, default_val) (0)
#define gpiod_line_set_value(line, val) printf("GPIO set to %d\n", val)
#define gpiod_chip_close(chip) (0)

#else
#include <gpiod.h>
#endif
