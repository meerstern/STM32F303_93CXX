


#include "stm32f3xx_hal.h"




/*	Define EEPROM TYPE	*
 * 			93CXX		*/
#define C86
//#define C56
//#define C46


#define CMD_READ 	0x02
#define CMD_WEN 	0x00
#define CMD_WRITE 	0x01


#ifdef C46
	#define CMD_SHIFT	9
#elif defined (C56)
	#define CMD_SHIFT	11
#elif defined (C86)
	#define CMD_SHIFT	13
#endif


#define MEM_DelayNUM	2
#define MEM_DeLay()		for(uint16_t i=0;i<MEM_DelayNUM;i++)asm("NOP")
#define MEM_Timeout		1000


void MEM_writeData(int16_t address,int8_t data);
int16_t MEM_readData(int16_t address);
void MEM_WDIS();
void MEM_WEN();
