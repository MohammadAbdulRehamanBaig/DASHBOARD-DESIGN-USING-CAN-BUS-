#include<LPC21xx.h>

#include"delay.h"

#include"LCD.h"

#include"types.h"

#include"macros.h"


#define LCD_DAT 16//data lines --P0.16-P0.23

//#define LCD_DAT 23

#define RS 9 //P0.8

#define RW 8 //P0.9

#define EN 10 //P0.10


void InitLCD(void)

{

	//P0.0-P0.10--By default GPIO pins

	IODIR0|=((1<<RS)|(1<<RW)|(1<<EN));
	WRITEBYTE(IODIR0,LCD_DAT,0XFF);
//	WRITEBYTE(IODIR1,LCD_DAT,0XFF);
	         //0x7ff

	//P0.0-P0.10 pis are sel for ouput dir

	delay_ms(20);//min 15 msec required

	CmdLCD(0x30);//8-bit mode enabling

	delay_ms(10);//min. of 5 msec

	CmdLCD(0x30);//8-bit mode enabling

	delay_ms(1);//min. of 160 usec

	CmdLCD(0x30);//8-bit mode enabling

	delay_ms(1);//min. of 160 usec

	CmdLCD(0x38);//8-bit with 5*7 font 1 lines disp

	CmdLCD(0x10);//Display off

	CmdLCD(0x01);//Clear display

	CmdLCD(0x06);//Set cursor move direction

	CmdLCD(0x0c);//Disp ON ,cursor ON with Blinking		0

}

void CmdLCD(u8 cmd)

{

	IOCLR0=1<<RS;//clear P0.8-->RS of LCD

	DispLCD(cmd);

}

void CharLCD(u8 dat)

{

	IOSET0=1<<RS;//SET P0.8-->RS of LCD

	DispLCD(dat);

}

void DispLCD(u8 val)

{

	IOCLR0=1<<RW;//clear P0.9--->RW of LCD

	WRITEBYTE(IOPIN0,LCD_DAT,val);//write cmd to P0.0-P0.7
//		WRITEBYTE(IOPIN1,LCD_DAT,val);

	IOSET0=1<<EN;//make (P0.10)En high

	delay_ms(2);//min. of 450 nsec expected

	IOCLR0=1<<EN;//clear P0.10--->EN of LCD

	delay_ms(5);

}

void StrLCD(unsigned char *ptr)

{

	while(*ptr!='\0')

		CharLCD(*ptr++);

}

void IntLCD(s32 num)

{

	u8 a[10];

	s8 i=0;

	//itoa

	if(num==0)

		CharLCD('0');

	else

	{

		if(num<0)

		{

			num=-num;

			CharLCD('-');

		}

		while(num>0)

		{

			a[i++]=num%10+48;

			num=num/10;

		}

		for(--i;i>=0;i--)

			CharLCD(a[i]);

	}

}

void FltLCD(f32 val)

{
     int integer;
	 int decimal;
	 integer=(int)val;
	 decimal=(val-integer)*100;
	 IntLCD(integer);
	 CharLCD('.');
	 IntLCD(decimal);
}

/*void StoreCustCharFont(void)

{

	//u8 i,LUT[]={0x00,0x00,0x04,0x0c,0x1c,0x1c,0x1c,0x00};;//bluetooth

	//u8 i,LUT[]={0x00,0x00,0x0e,0x0a,0x0e,0x00,0x00,0x00};;//square box
	//u8 i,LUT[]={0xff,0x11,0x11,0x11,0x11,0x11,0x11,0xff};;//box for battery
	u8 i,LUT[]={0x00,0xfb,0xfb,0x11,0x0e,0x00,0x00,0x00};;
	//u8 j
	// u8 LET[]=xff,0x11,0x11,0x11,0x11,0x11,0xff,0xff};;
	//LUT[]={0xff,0x11,0x11,0x11,0x11,0xff,0xff,0xff};;
	for(i=0;i<8;i++)
	{

		CharLCD(LUT[i]);
		//CharLCD(LET[i]);
		}
	for(j=0;j<8;j++)
	{
		CharLCD(SET[j]);
		} 
	u8 j,LET[]={0xff,0x11,0x11,0x11,0x11,0x11,0xff,0xff};;
	for(j=0;j<8;j++)
	{
		CharLCD(LET[j]);
		}


} */
void StoreCustCharFont(void)

{
	u8 i,j;
	u8 fuel[5][8]={
					{0x1F,0x11,0x10,0x10,0X10,0X11,0X1F},
					{0x1F,0x11,0x10,0x10,0X1C,0X11,0X1F},
					{0x1F,0x11,0x10,0x1C,0X1C,0X11,0X1F},
					{0x1F,0x11,0x1C,0x1C,0X1C,0X11,0X1F},
					{0x1F,0x11,0x1F,0x1F,0X1F,0X11,0X1F},
	};
	for(j=0;j<5;j++)
	{
	   CmdLCD(0X40 + (j*8));
	   for(i=0;i<8;i++)
	   CharLCD(fuel[j][i]) ;

		//CharLCD(LUT[i]);
		//CharLCD(LET[i]);
		}
	/*for(j=0;j<8;j++)
	{
		CharLCD(SET[j]);
		} */
	/*u8 j,LET[]={0xff,0x11,0x11,0x11,0x11,0x11,0xff,0xff};;
	for(j=0;j<8;j++)
	{
		CharLCD(LET[j]);
		}
	  */



}
