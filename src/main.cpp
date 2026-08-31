#include <iostream>
#include <cstdint>
#include <unistd.h>
#include "spi.hpp"

int main(void) {
    SPI spi;

    // Start in reset
    spi.reset();

    // Set FREQ0 to 1 MHz with a 25 MHz master clock
    spi.set_frequency(1000000, 25.0e6);

    // Exit reset, enable B28 mode, use FREQ0
    spi.write(CTRL_B28);

    sleep(1);

    return 0;
}
