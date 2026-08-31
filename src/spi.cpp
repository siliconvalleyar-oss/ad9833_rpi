#include "spi.hpp"
#include <unistd.h>

SPI::SPI() {
    if (!bcm2835_init()) {
        fprintf(stderr, "bcm2835 init failed\n");
        exit(EXIT_FAILURE);
    }

    // Configure SPI0 as master, mode 0, MSB first, 8 bits per word
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16); // ~1.56 MHz @ 25 MHz core
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

SPI::~SPI() {
    bcm2835_spi_end();
    bcm2835_close();
}

// Transmit a 16-bit word: 2 control bits (MSBs) + 14 data bits.
// Sends MSB byte first.
void SPI::write(uint16_t cmd) {
    uint8_t buf[2] = { (uint8_t)(cmd >> 8), (uint8_t)(cmd & 0xFF) };
    bcm2835_spi_transfern((char*)buf, 2);
}

// Write to a given AD9833 register: control, frequency (LSB/MSB pairs) or phase.
void SPI::select(uint8_t reg) {
    write((uint16_t)(reg << 14));
}

void SPI::set_frequency(uint32_t freq, double mclk) {
    if (freq >= (uint32_t)(mclk / 2.0))
        freq = (uint32_t)(mclk / 2.0) - 1;

    // 28-bit frequency word: freq = value * mclk / 2^28
    uint32_t word = (uint32_t)(((uint64_t)freq * (1UL << 28)) / (uint64_t)mclk) & 0x0FFFFFFF;

    // Select FREQ0, then write the 14 LSBs (B28 mode -> LSB first, then MSB)
    select(CMD_FREQ0);
    write((uint16_t)(word & 0x3FFF));
    write((uint16_t)((word >> 14) & 0x3FFF));
}

void SPI::set_phase(uint16_t phase) {
    phase &= PHASE_REG_SIZE;
    select(CMD_PHASE0);
    write(phase);
}

void SPI::reset() {
    write(CTRL_RESET);
}
