#include<LPC21xx.h>
#include"types.h"
void InitLCD(void);
void CmdLCD(u8);
void CharLCD(u8);
void DispLCD(u8);
void StrLCD(unsigned char*);
void IntLCD(s32);
void FltLCD(f32);
void StoreCustCharFont(void);
