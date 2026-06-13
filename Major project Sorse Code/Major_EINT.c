#include <LPC21xx.h>
#include "delay.h"
#include "Major_Can.h"
#include "types.h"
#define OFF 0
#define LEFT 1
#define RIGHT 2
#define LED_START 23
#define LED_MASK (0xFF << LED_START)


 CANF rxF;
volatile u8 indicator = OFF;
volatile u8 pos = 0;
void blink_left_step(void);
void blink_right_step(void);
int main()
{
    Init_CAN1();
    IODIR1 |= LED_MASK;
    IOSET1 = LED_MASK; // LEDs OFF
    while (1)
    {
	  
      //   -------- CAN RX -------- 

        if (C1GSR & (1 << 0))
        {
					//IOCLR1=(1<<23);
            CAN1_Rx(&rxF);
            C1CMR = (1 << 2); // MUST release RX buffer
            if (rxF.ID == 0x11)
            {

                if (rxF.Data1 == 'L')
                {
                    indicator = LEFT;

                    //pos = 0;
										pos=7;

                }

                else if (rxF.Data1 == 'R')

                {

                    indicator = RIGHT;

                    //pos = 7;
										pos=0;

                }

                else if (rxF.Data1 == 'O')
                {
                    indicator = OFF;
                    pos = 0;
                    IOSET1 = LED_MASK; // force OFF
                }

            }

        }
        /* -------- LED ACTION -------- */

        if (indicator == LEFT)
        {
            blink_left_step();

        }
        else if (indicator == RIGHT)
        {
            blink_right_step();
        }
        else
        {
            IOSET1 = LED_MASK;
        }
        delay_ms(50);
    }	 
/*	if(C1GSR &(1<<0))
	{
	//IOSET1=LED_MASK;
	IOCLR1=(1<<23);
	CAN1_Rx(&rxF);
	delay_ms(500);
	IOSET1=(1<<23);
	//delay_ms(500);
	}
	}  */
}

/* -------- LEFT BLINK LED SHIFTING -------- */

void blink_right_step(void)
{
    IOSET1 = LED_MASK;
    IOCLR1 = (1 << (LED_START + pos));

	 pos++;

    if (pos >= 8)
        pos = 0;
		
		
		
}
/* -------- RIGHT BLINK LED SHIFTING -------- */

void blink_left_step(void)
{
    IOSET1 = LED_MASK;
	// IOCLR1 = (1 << (LED_START + pos));	//siri
    if (pos == 0)
        pos = 7;
    else
        pos--;
    IOCLR1 = (1 << (LED_START + pos));
}
