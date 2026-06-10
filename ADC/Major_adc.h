#ifndef _ADC_H_

#define _ADC_H_


#include "types.h"

void initadc(u32);

f32 Read_ADC(u8 chNo,f32 *,u32 *);
void CfgPinFunc(int,int,int);


#endif
