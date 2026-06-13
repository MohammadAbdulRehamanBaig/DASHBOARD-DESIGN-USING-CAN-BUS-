#ifndef _ADC_H_
#define _ADC_H_

#include "types.h"

// Initialize ADC channel
void initadc(u32);

// Read ADC value and equivalent analog voltage
f32 Read_ADC(u8 chNo, f32 *, u32 *);

// Configure pin function (GPIO/ADC/UART etc.)
void CfgPinFunc(int,int,int);

#endif