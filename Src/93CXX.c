


#include "93CXX.h"
#include "spi.h"


void sendCommand(int8_t cmd, int16_t address);
void checkReady();


int16_t MEMrData[1];
int16_t MEMsData[1];


int16_t spi_transfer(int16_t data)
{
	MEMsData[0]=data;
	HAL_SPI_TransmitReceive(&hspi1,
								(uint8_t*)&MEMsData,
								(uint8_t*)&MEMrData,
								1,1000);
	return MEMrData[0];
}

void sendCommand(int8_t cmd, int16_t address)
{

	int16_t sendcmd;
	sendcmd = (1 << CMD_SHIFT) | (cmd << (CMD_SHIFT-2)) | address;
	spi_transfer((int8_t)(sendcmd >> 8));
	MEM_DeLay();
	spi_transfer((int8_t)(sendcmd & 0xff));
	MEM_DeLay();

}

void checkReady()
{
	uint16_t timecnt=0;

	if(spi_transfer(0)==0) {
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,0);
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,1);

		while(spi_transfer(0)==0) {
			MEM_DeLay();
			timecnt++;
			if(timecnt>MEM_Timeout)break;
    	}
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,0);
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,1);
  }
}

void MEM_writeData(int16_t address,int8_t data)
{
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,1);
  checkReady();
  sendCommand(CMD_WRITE,address);
  spi_transfer(data);
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,0);
}

int16_t MEM_readData(int16_t address)
{
	int16_t res;

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,1);
	checkReady();
	sendCommand(CMD_READ,address);
	res = spi_transfer(0xff);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,0);
	return res;
}

void MEM_WEN(){

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,1);
	sendCommand(CMD_WEN,0x600);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,0);

}

void MEM_WDIS(){

	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,1);
	sendCommand(CMD_WEN,0x000);
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin,0);

}
