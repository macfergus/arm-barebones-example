#include <stdint.h>

void busywait() {
    uint32_t countdown = 1000000;
    while (countdown > 0) {
        --countdown;
    }
}

// GPIO pin control are in memory mapped registers. The pins are organized
// in banks of 32. The bits in this structure control each individual pin
// See p 799 in datasheet for details
struct portctl {
    uint32_t DIR;
    uint32_t DIRCLR;
    uint32_t DIRSET;
    uint32_t DIRTGL;
    uint32_t OUT;
    uint32_t OUTCLR;
    uint32_t OUTSET;
    uint32_t OUTTGL;
    uint32_t IN;
    uint32_t CTRL;
    uint32_t WRCONFIG;
    uint32_t EVCTRL;
    uint8_t PMUX[16];
    uint8_t PINCFG[32];
};
// You might want to write this as:
// volatile struct portctl* PORTA = 0x41008000;
// but that creates a global variable, and we've left out the parts we need to
// initialize globals.
#define PORTA ((volatile struct portctl*)(0x41008000))

int main() {
    // The metro LED is on D13 which is PA16
    uint32_t pinbit = (1 << 16);
    // Switch the pin to output
    PORTA->DIRSET = pinbit;
    while (1) {
        // set pin high
        PORTA->OUTSET = pinbit;
        busywait();
        // set pin low
        PORTA->OUTCLR = pinbit;
        busywait();
    }
}
