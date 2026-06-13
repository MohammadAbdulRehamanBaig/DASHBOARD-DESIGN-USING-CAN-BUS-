#include<LPC21XX.h>
#include"types.h"
#include"Major_adc_defines.h"
#include"Major_adc.h"
#include "Major_fuel.h"
#include "delay.h"
#include "Major_Can.h"
#include "Major_candefines.h"
#include "Major_lcd.h"

f32 aR1;
u32 adcdval;
u8 fuel;

f32 voltage;
CANF txF;
int main(void) 

{	
 //s	InitLCD();
	Init_CAN1();
	initadc(0);
	txF.ID=1;
	txF.BFV.RTR=0;
	txF.BFV.DLC=4;
	while(1) 
	{	 
		//CmdLCD(0x01);
		//Read_ADC(1,&aR1,&adcdval);
		Read_ADC(0,&voltage,&adcdval);
		fuel=((adcdval*100)/1023);
		if(fuel>100)
		fuel=100;
	
		
	txF.Data1=fuel;				              
	//txF.Data2=0;
	 CAN1_Tx(txF);		  
		delay_ms(100);
	//	CmdLCD(0x01);
	}
}
