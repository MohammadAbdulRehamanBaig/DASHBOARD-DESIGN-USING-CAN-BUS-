#ifndef _ADC_DEFINES_H_
#define _ADC_DEFINES_H_

// ADC channel selection bits
#define CH0 0x01
#define CH1 0x02
#define CH2 0x04
#define CH3 0x08

// Select channels 0 and 1
#define CHANNEL_SEL CH0|CH1

// Oscillator frequency = 12 MHz
#define FOSC 12000000

// CPU clock frequency = 60 MHz
#define CCLK (5 * FOSC)

// Peripheral clock frequency = 15 MHz
#define PCLK CCLK/4

// ADC clock frequency
#define ADCLK 3750000

// ADC clock divider value
#define CLKDIV (((PCLK/ADCLK)-1)<<8)

// Power-down control bit
#define PDN_BIT (1<<21)

// ADC conversion start bit position
#define ADC_START_BIT 24

// ADC conversion completion bit position
#define DONE_BIT 31

#endif