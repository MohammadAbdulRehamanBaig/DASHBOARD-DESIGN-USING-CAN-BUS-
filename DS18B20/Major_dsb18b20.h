//DS18B20 TEMPRATURE SENSOR HEADER
#ifndef DS18B20_H
#define DS18B20_H
u8 ResetDs18b20(void);
unsigned char ReadBit(void);
void WriteBit(unsigned char Databit);
unsigned char ReadByte(void);
void WriteByte(unsigned char DatOut);
int ReadTemp(void); // siri
#endif