//can.c
#include <LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "Major_candefines.h"
#include "Major_can.h"
//#include "pin_connect_block.h"

void Init_CAN1(void)
{
    //cfg p0.25 as CAN1_RX pin(RD1)
    PINSEL1=(1<<((25-16)*2));
//      CfgPortPinFunc(0,25,RD1_PIN_0_25);
    //Reset CAN1 controller
    C1MOD|=1<<RM_BIT;
    //all received messages are accepted
          AFMR&=~(1<<AccOFF_BIT);
    AFMR|=(1<<AccBP_BIT);
          //Set baud Rate for CAN
    C1BTR=BTR_LVAL;
    //Enable CAN1 controller
    C1MOD&=~(1<<RM_BIT);
}

void CAN1_Tx(CANF txF)
{
   // Checking that the TX buffer is empty
   while(((C1GSR>>TBS1_BIT)&1)==0);
   // Cfg Tx ID
   C1TID1=txF.ID;
   // Cfg RTR & DLC     
   C1TFI1=((txF.BFV.RTR<<RTR_BIT)|
          (txF.BFV.DLC<<DLC_BITS));
    //Check whether Data/Remote Frame to Transmit
   if(txF.BFV.RTR!=1)
   {
     //if D Frame,wr to data transmit buffers
           C1TDA1= txF.Data1; /*bytes 1-4 */
           C1TDB1= txF.Data2; /*bytes 5-8*/
   }
   //Select Tx Buff 1 & Start Xmission
   C1CMR|=((1<<STB1_BIT)|(1<<TR_BIT));
   //wait until tx complete
   while(((C1GSR>>TCS1_BIT)&1)==0);
}

void CAN1_Rx(CANF *rxF)
{
  //wait for CAN frame recv status
  while(((C1GSR>>RBS_BIT)&1)==0);
  //read 11-bit CANid of recvd frame.
  rxF->ID=C1RID;
  //& read & extract data/remote frame status
  rxF->BFV.RTR=((C1RFS>>RTR_BIT)&1);
  //& extract data length
  rxF->BFV.DLC=((C1RFS>>DLC_BITS)&15);
  //check if recvd frame is data frame,
  if(rxF->BFV.RTR==0)
  {
   //extract data bytes 1-4
   rxF->Data1=C1RDA;
   //extract data bytes 5-8
   rxF->Data2=C1RDB;
  }
  // Release receive buffer command
  C1CMR|=(1<<RRB_BIT);
}
