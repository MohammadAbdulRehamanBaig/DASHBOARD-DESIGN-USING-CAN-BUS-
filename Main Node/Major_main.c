#include<lpc21xx.h>
#include "Major_adc.h"
#include "Major_adc_defines.h"
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

//volatile u32 percent=0;
u8 percent=0;
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
{//	 IODIR1|=1<<16;
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
	//IODIR0|=1<<1;

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
        CmdLCD(0xD4);
       StrLCD("Temp ");
       //CharLCD(7);
        StrLCD(" : ");
        IntLCD(tp);
        StrLCD(" C   ");

		{

        CAN1_Rx(&rxF);
		 CmdLCD(0x94);
	 StrLCD(" Fuel: ");
	IntLCD(rxF.Data1);
		CharLCD('%');
	
	percent = rxF.Data1;
		if(percent<=25){
			CharLCD(2);
			CharLCD(' ');
			CharLCD(' ');
			CharLCD(' ');
		}
		else if(percent<=50){
			CharLCD(2);
			CharLCD(3);
			CharLCD(' ');
			CharLCD(' ');
		}
		else if(percent<=75){
			CharLCD(2);
			CharLCD(3);
			CharLCD(4);
			CharLCD(' ');
		}else{
			CharLCD(2);
			CharLCD(3);
			CharLCD(4);
		 	CharLCD(5);
		}
               

        delay_ms(200);
		//}
		}
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
