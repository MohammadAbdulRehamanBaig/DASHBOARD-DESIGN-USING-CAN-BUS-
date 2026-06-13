#include<lpc21xx.h>
#include "Major_adc.h"
#include "Major_adc_defines.h"
//#include "Major_rtc_def.h"
//#include "Major_rtc_defines.h"
#include "Major_lcd.h"
#include "Major_adc.h"
#include "Major_adc_defines.h"
#include "Major_dsb18b20.h"
#include "types.h"
#include "delay.h"
#include"Major_Can.h"
u32 adcvalue;
f32 voltage;
int temp;
int Ful;
u8 tp,tpd;

/*int main()
{
   InitLCD();
   PINSEL0 &=~(3<<30);
   //CmdLCD(0x80);
   //Init_ADC(CH1);
   while(1)
   {
      Read_ADC(CH1,&voltage,&adcvalue);
     CmdLCD(0x80);
	 FltLCD(voltage);
	 delay_s(5);
	 CmdLCD(0x01);
	 CmdLCD(0x80);
	 delay_ms(500);
	 CharLCD('#');
	 delay_ms(100);
	 IntLCD(adcvalue);
	 delay_ms(200);
	 }
	 //while(1);
	 while(1)
	 {
	   	CmdLCD(0x80);
	    if(ResetDs18b20())
	    StrLCD("sensor ok");
		else
		StrLCD("sensor not okay");
		delay_ms(1000);
	    temp=ReadTemp();
		IntLCD(temp);
		CmdLCD(0x80);
		StrLCD("temp :");
		tp=temp>>4;
		tpd=(temp&0x0f)*625/1000;
		//tpd=(temp& 0x08) ? '5' : '0';
		IntLCD(tp);
		//delay_ms(500);
		//StrLCD("hello");
		//delay_ms(500);
		 CharLCD('.');
		IntLCD(tpd);
		CharLCD(' ');
		CharLCD('c');
		//delay_s(1);
		CmdLCD(0x01);
		CmdLCD(0x80);	
	  }	


} */
/*  
int main()
{			    InitLCD();
				Init_ADC(1);
				while(1)
				{
			    CmdLCD(0x01);
				delay_s(2);
                StrLCD("DASHBOARD");
                CmdLCD(0xc0);
                StrLCD("T:");
                temp=ReadTemp();
                IntLCD(temp>>4);
                CharLCD(223);
                CharLCD('C');
			//	Read_ADC(CH1,&voltage,&adcvalue);
			//	CmdLCD(0xc0);
			//	Ful=Read_Fuel(); 
				}				 
}
  */
/*
int main()
{
int fuel;
InitLCD();
delay_us(100);
Init_ADC(CH1);
 while(1)
 {
//Read_ADC(CH1,&voltage,&adcvalue);
CmdLCD(0x80);
 StrLCD("F:");
//Ful=(adcvalue);
fuel= 4; //Read_Fuel();

IntLCD(fuel);
CharLCD('%');

delay_s(3);
}
}*/

void temp_symbol(){
   u8 i;
    u8 tem[8]   ={0x0E,0x0A,0x0A,0x1f,0x1f,0x1f,0x1f,0x1F};
    CmdLCD(0x78);
    for(i=0;i<8;i++){
       CharLCD(tem[i]);
    }
}
/* -------- Defines -------- */
#define OFF   0
#define LEFT  1
#define RIGHT 2

u32 convert(u32 x){
  if(x<1){
    x=0;
  }
  if(x>330){
    x=330;
  }
  return (x*100)/330;
}

volatile u32 percent=0;
f32 voltage;
u8 battery_percent,i;
u32 level=0;

/* -------- Globals -------- */
volatile u8 indicator_status = OFF;
CANF txF;
CANF rxF;
/* -------- Custom LCD chars -------- */
u8 left_arrow[8]  = {0x03,0x07,0x0F,0x1F,0x0F,0x07,0x03,0x01};
u8 right_arrow[8] = {0x18,0x1C,0x1E,0x1F,0x1E,0x1C,0x18,0x10};

/* -------- Prototypes -------- */
void LoadCustomChars(void);
void sendsignal(void);
void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

int temp;
u8 tp, tpd;
int main(void)
{	 //IODIR1|=1<<16;
	 InitLCD();
	 CmdLCD(0x80);
	 CharLCD('5');
    Init_CAN1();
    //initlcd();
    CmdLCD(0x80);
   StrLCD(" <<< DASH BOARD >>> ");
    temp_symbol();
   CmdLCD(0xC0);
   StrLCD("Indicator ");
    /* Load custom arrows */
    LoadCustomChars();
   /* StoreCustCharFont(0); 
    StoreCustCharFont1(25);
    StoreCustCharFont2(50);
    StoreCustCharFont3(75);
    StoreCustCharFont4(100);  */

    /* Show steady <> initially */
    CmdLCD(0xC0 + 13);
    CharLCD(5);      // <
    CmdLCD(0xC0 + 15);
    CharLCD(6);      // >

    /* -------- EINT config -------- */
    CfgPinFunc(0,1,3);   // EINT0
    CfgPinFunc(0,3,3);   // EINT1

    EXTMODE  |= (1<<0) | (1<<1);
    EXTPOLAR &= ~((1<<0) | (1<<1));
    EXTINT    = (1<<0) | (1<<1);

    VICVectCntl0 = (1<<5) | 14;
    VICVectAddr0 = (u32)eint0_isr;

    VICVectCntl1 = (1<<5) | 15;
    VICVectAddr1 = (u32)eint1_isr;

    VICIntEnable = (1<<14) | (1<<15);

    while(1)
    {

        /* LEFT indicator blink */
        if(indicator_status == LEFT)
        {
           CmdLCD(0xC0 + 13);
            CharLCD(5);          // <
            delay_ms(300);
           CmdLCD(0xC0 + 13);
           CharLCD(' ');

           CmdLCD(0xC0 + 15);
            CharLCD(6);          // > steady
        }

        /* RIGHT indicator blink */
        else if(indicator_status == RIGHT)
        {
            CmdLCD(0xC0 + 13);
            CharLCD(5);          // < steady

           CmdLCD(0xC0 + 15);
           CharLCD(6);          // >
            delay_ms(300);
            CmdLCD(0xC0 + 15);
           CharLCD(' ');
        }

        /* OFF state */
        else
        {
            CmdLCD(0xC0 + 13);
            CharLCD(5);
           CmdLCD(0xC0 + 15);
            CharLCD(6);
        }

        delay_ms(100);
        //cmdlcd(0xD4);
        temp = ReadTemp();
        tp  = temp >> 4;
        //tpd = (temp & 0x08) ? '5' : '0';
        CmdLCD(0xD4);
       StrLCD("Temp ");		  .
       //CharLCD(7);
        StrLCD(" : ");
        IntLCD(tp);
       //CharLCD('.');
      // CharLCD(tpd);
        StrLCD(" C   ");

      //if (C1GSR & (1 << 0))
      //{
        /****Fuel Node****/
		//if(rxF.ID==1)
		{

        CAN1_Rx(&rxF);
		 CmdLCD(0x94);
		  // StrLCD(" id");
		  // FltLCD(rxF.ID);
		   	//CmdLCD(0x9a);
	   StrLCD(" d1: ");
		  IntLCD(rxF.Data1);




	/*
        Fuel message 
        if(rxF.ID == 0x301)
        {

               // percent = convert(rxF.Data1);
			   percent =   (rxF.Data1);
                CmdLCD(0x94);
               StrLCD("FUEL ");
                FltLCD(percent);
                StrLCD("%   ");
                if(percent <= 0){
                         level=0;
                }
                else if(percent <= 25){
                     level=1 ;
                }
                else if(percent <= 50){
                      level=2;
                }
                else if(percent <= 75){
                       level=3;
                }
                else {
                        level=4; 
                }

                CmdLCD(0x94+13);
               CharLCD(level);
        }
        }*/

        delay_ms(200);
		//}
		}							.
    }
}

/* -------- EINT0 : LEFT -------- */
void eint0_isr(void) __irq
{
		IOPIN1^=(1<<16);
    indicator_status = (indicator_status == LEFT) ? OFF : LEFT;
    sendsignal();

    EXTINT = (1<<0);
    VICVectAddr = 0;
}

/* -------- EINT1 : RIGHT -------- */
void eint1_isr(void) __irq
{
    indicator_status = (indicator_status == RIGHT) ? OFF : RIGHT;
    sendsignal();

    EXTINT = (1<<1);
    VICVectAddr = 0;
}

/* -------- CAN TRANSMIT -------- */
void sendsignal(void)
{
    txF.ID = 0x11;
    txF.BFV.RTR = 0;
    txF.BFV.DLC = 1;
    txF.Data1   = (indicator_status == LEFT) ? 'L' :
                  (indicator_status == RIGHT) ? 'R' : 'O';
    CAN1_Tx(txF);
}

/* -------- Load CGRAM arrows -------- */
void LoadCustomChars(void)
{
    u8 i;

   CmdLCD(0x68);              // char 0 = <
    for(i=0;i<8;i++) CharLCD(left_arrow[i]);

   CmdLCD(0x70);              // char 1 = >
    for(i=0;i<8;i++) CharLCD(right_arrow[i]);
}
