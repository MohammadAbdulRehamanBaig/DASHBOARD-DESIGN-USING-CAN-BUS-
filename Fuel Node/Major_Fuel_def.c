#include<LPC21XX.h>
#include"types.h"
#include"Major_adc_defines.h"
#include"Major_adc.h"
#include "Major_fuel.h"
#include "delay.h"
#include "Major_Can.h"
#include "Major_candefines.h"
#include "Major_lcd.h"
/*CANF txF;
u32 adcDVal;
f32 eAR;
int main(void)
{
	int fuel;
	InitLCD();
	Init_CAN1();
	Init_ADC();
	txF.ID=0x301;
	txF.bfv.RTR=0;
	txF.bfv.DLC=4;
	while(1)
	{
	  CmdLCD(0x01);
	  Read_ADC(CH1);
	  //fuel=((adcDVal/1023)*100);
	  fuel =(adcDVal*100)/1023;
	  IntLCD(adcDVal);
	  //StrLCD("hello");
	  IntLCD(fuel);
	  txF.Data1=fuel;
	  txF.Data2=0;
	  CAN1_Tx(txF);
	  delay_ms(500);
	  //return fuel;  
	 }
}
  */
//#include "types.h"

//#include "adc.h"

//#include "adc_defines.h"

//#include "uart0.h"

//#include "delay.h"


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
		//CmdLCD(0x80);
	//	StrLCD("V");
		//CharLCD('%');
	//	CharLCD(':');
		//voltage=(aR0*3.3)/1023;
	//	FltLCD(voltage);
		//fuel=(voltage*100)/1023;
	//	CmdLCD(0x88);
		//FltLCD(fuel);
	//	StrLCD("A:");
	//	IntLCD(adcdval);
		//CmdLCD(0x80);
		//StrLCD("fuel");
		//CharLCD('%');
		//CharLCD(':');
	/*	fuel=((aR1*100)/1023);
		if(fuel>100)
		fuel=100;
		CmdLCD(0x80);
		StrLCD("fuel");
		CharLCD('%');
		CharLCD(':');*/
//s	CmdLCD(0xc0);
	//s	StrLCD("fuel");
	//s	IntLCD(fuel);
	//s	CharLCD('%');
	//	IntLCD(aR1);	                                                  
	txF.Data1=fuel;				              
	//txF.Data2=0;
	 CAN1_Tx(txF);		  
		delay_ms(100);
	//	CmdLCD(0x01);
	}
}
