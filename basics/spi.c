#include "stm32f4xx.h"
#include "spi.h"


void SPI_Send_One_Byte(SPI_TypeDef *SPIx, uint8_t byte)
{
    /* wait untill DR register is empty */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

    /* Send a Byte through the SPI peripheral */
    SPI_I2S_SendData(SPIx, byte);    
}
uint8_t SPI_Read_One_Byte(SPI_TypeDef *SPIx)
{
    /* wait untill Receive one byte */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    /* Receive a Byte through the SPI peripheral */
    return SPI_I2S_ReceiveData(SPIx);
}


