#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "spi.hpp"

int main(void) {
SPI *spi = new (SPI);
uint16_t address = 0b0101010101010101;

//spi->read_write(2,address);
//spi->read_write(2,0x0100);
sleep(0.1);


//spi->read_write(2,0x2100);
//spi->read_write(2,0x80c6);
//spi->read_write(2,0x4000);
//spi->read_write(2,0xC000);
spi->write(0x2000);


return 0;
}
