#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <bcm2835.h>

#define SPI_DEVICE "SPI0"

// AD9833 control register bits (D15:D14 select register)
#define CMD_CTRL    0b00
#define CMD_FREQ0   0b01
#define CMD_FREQ1   0b10
#define CMD_PHASE0  0b11
#define CMD_PHASE1  0b11 // with D13 set selects PHASE1

// AD9833 control register bits (D13:D0)
#define CTRL_B28       (1 << 13)
#define CTRL_HLB       (1 << 12)
#define CTRL_FSEL      (1 << 11)
#define CTRL_PSEL      (1 << 10)
#define CTRL_PIN_SW    (1 << 9)
#define CTRL_RESET     (1 << 8)
#define CTRL_SLEEP1    (1 << 7)
#define CTRL_SLEEP12   (1 << 6)
#define CTRL_OPBITEN   (1 << 5)
#define CTRL_SIGN_PIB  (1 << 4)
#define CTRL_DIV2      (1 << 3)
#define CTRL_MODE      (1 << 1)

#define FREQ_REG_SIZE  0x2000 // frequency register is 28 bits (2 x 14-bit writes)
#define PHASE_REG_SIZE 0x0FFF // phase register is 12 bits

#define FSYNC_ACTIVE  0x00
#define FSYNC_RELEASE 0xFF

class SPI {
public:
    SPI();
    ~SPI();

    void write(uint16_t cmd);
    void set_frequency(uint32_t freq, double mclk = 25.0e6);
    void set_phase(uint16_t phase);
    void reset();

private:
    void select(uint8_t ctrl);
    void write_word(uint8_t high, uint8_t low);
};
