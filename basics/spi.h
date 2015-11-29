#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f4xx.h"


void SPI_Send_One_Byte(SPI_TypeDef *SPIx, uint8_t byte);
uint8_t SPI_Read_One_Byte(SPI_TypeDef *SPIx);

#endif //__SPI_H__

