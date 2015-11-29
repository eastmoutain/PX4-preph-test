#include "preph.h"
#include <stdio.h>
#include "spi.h"
#include "SysTick.h"

union axi{
    uint16_t axi_short;
    uint8_t axi_h_l[2];
};

struct six_axi{
    union axi acc_X;
    union axi acc_Y;
    union axi acc_Z;
    union axi gyro_X;
    union axi gyro_Y;
    union axi gyro_Z;
};

static void delay(uint32_t dot)
{
    while(dot--);
}
static void SPI1_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    /* Enable the SPI periph */
    RCC_APB2PeriphClockCmd(MPU60XX_SPI_CLK, ENABLE);

    /* Enable SCK, MOSI and MISO GPIO clocks */
    RCC_AHB1PeriphClockCmd(MPU60XX_SPI_SCK_GPIO_CLK | \
                           MPU60XX_SPI_MISO_GPIO_CLK | \
                           MPU60XX_SPI_MOSI_GPIO_CLK, \
                           ENABLE);

    /* Enable CS  GPIO clock */
    RCC_AHB1PeriphClockCmd(MPU60XX_SPI_CS_GPIO_CLK, ENABLE);

    /* Enable INT1 GPIO clock */
    RCC_AHB1PeriphClockCmd(MPU60XX_SPI_INT1_GPIO_CLK, ENABLE);


    GPIO_PinAFConfig(MPU60XX_SPI_SCK_GPIO_PORT, MPU60XX_SPI_SCK_SOURCE, MPU60XX_SPI_SCK_AF);
    GPIO_PinAFConfig(MPU60XX_SPI_MISO_GPIO_PORT, MPU60XX_SPI_MISO_SOURCE, MPU60XX_SPI_MISO_AF);
    GPIO_PinAFConfig(MPU60XX_SPI_MOSI_GPIO_PORT, MPU60XX_SPI_MOSI_SOURCE, MPU60XX_SPI_MOSI_AF);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = MPU60XX_SPI_SCK_PIN;
    GPIO_Init(MPU60XX_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /* SPI  MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  MPU60XX_SPI_MOSI_PIN;
    GPIO_Init(MPU60XX_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /* SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Pin = MPU60XX_SPI_MISO_PIN;
    GPIO_Init(MPU60XX_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    /* SPI configuration -------------------------------------------------------*/
    SPI_I2S_DeInit(MPU60XX_SPI);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    
    SPI_Init(MPU60XX_SPI, &SPI_InitStructure);

    /* Enable SPI1  */
    SPI_Cmd(MPU60XX_SPI, ENABLE);

    /* Configure GPIO PIN for Lis Chip select */
    GPIO_InitStructure.GPIO_Pin = MPU60XX_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU60XX_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

    /* Deselect : Chip Select high */
    GPIO_SetBits(MPU60XX_SPI_CS_GPIO_PORT, MPU60XX_SPI_CS_PIN);

    /* Configure GPIO PINs to detect Interrupts */
    GPIO_InitStructure.GPIO_Pin = MPU60XX_SPI_INT1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(MPU60XX_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);
}


/*  MPU6000 Chip enalbe     */
static void MPU6000_chip_select(void)
{
    GPIO_ResetBits(MPU60XX_SPI_CS_GPIO_PORT, MPU60XX_SPI_CS_PIN);
}
/*  MPU6000 Chip disable    */
static void MPU6000_chip_unselect(void)
{
    GPIO_SetBits(MPU60XX_SPI_CS_GPIO_PORT, MPU60XX_SPI_CS_PIN);

}
/*  SPI1 Write one byte to device*/
static uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData);
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1);			    
}
/*  MPU6000 Write @value to @reg    */
static uint8_t MPU6000_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;

    MPU6000_chip_select();
	status=SPI1_ReadWriteByte(reg);
	SPI1_ReadWriteByte(value);
    MPU6000_chip_unselect();

	return(status);
}
/*  MPU6000 Read one byte from @reg     */
static uint8_t MPU6000_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
    
	MPU6000_chip_select();

	SPI1_ReadWriteByte(reg|0x80); 
	reg_val=SPI1_ReadWriteByte(0xff);
    
    MPU6000_chip_unselect();
    
	return(reg_val);
}



static void MPU6000_Init(void)
{
    static uint8_t init_data[] = {
                    0x00,
                    0x07,
                    0x07,
                    0x18,
                    0x01 
    };
    
    uint8_t *p = init_data;
    
    MPU6000_Write_Reg(MPUREG_PWR_MGMT_1, p[0]);
    MPU6000_Write_Reg(MPUREG_SMPLRT_DIV, p[1]);
    MPU6000_Write_Reg(MPUREG_CONFIG, p[2]);
    MPU6000_Write_Reg(MPUREG_GYRO_CONFIG, p[3]);
    MPU6000_Write_Reg(MPUREG_ACCEL_CONFIG, p[4]);
                    
    delay(1000);
    delay(1000);
                    
}

static uint16_t get_acc_X(void)
{
    union axi acc_X;
    
     static uint8_t accX_addr[] = {
        MPUREG_ACCEL_XOUT_H,
        MPUREG_ACCEL_XOUT_L,
    };
    uint8_t *p = acc_X.axi_h_l;
    
    p[0] = MPU6000_Read_Reg(accX_addr[0]);
    p[1] = MPU6000_Read_Reg(accX_addr[1]);

    return acc_X.axi_short;
}

static uint16_t get_acc_Y(void)
{
    union axi acc_Y;
    
    static uint8_t accY_addr[] = {
        MPUREG_ACCEL_YOUT_H,
        MPUREG_ACCEL_YOUT_L,
    };
    uint8_t *p = acc_Y.axi_h_l;
    
    p[0] = MPU6000_Read_Reg(accY_addr[0]);
    p[1] = MPU6000_Read_Reg(accY_addr[1]);

    return acc_Y.axi_short;
}

static uint16_t get_acc_Z(void)
{
    union axi acc_Z;
    
    static uint8_t accZ_addr[] = {
        MPUREG_ACCEL_ZOUT_H,
        MPUREG_ACCEL_ZOUT_L
    };
    
    uint8_t *p = acc_Z.axi_h_l;
    
    p[0] = MPU6000_Read_Reg(accZ_addr[0]);
    p[1] = MPU6000_Read_Reg(accZ_addr[1]);
    
    return acc_Z.axi_short;
}

void test_case_mpu6000(void)
{
    struct six_axi six_axi_data;
    int i;
    uint8_t tmp;
    
    printf("   %s start running\r\n", __FUNCTION__);
    
    SPI1_Init();
    
    MPU6000_Init();
    
    //mpu6000_read(0x75);
    printf("addr:0x75(MPUREG_WHOAMI) = 0x%02x\r\n", MPU6000_Read_Reg(MPUREG_WHOAMI));
 
 #if 1   
    for (i = 0; i < 20; i++) {
        six_axi_data.acc_X.axi_short = get_acc_X();
        six_axi_data.acc_Y.axi_short = get_acc_Y();
        six_axi_data.acc_Z.axi_short = get_acc_Z();
        
        printf("acc_X: %d, acc_Y: %d, acc_Z:%d\r\n",
                six_axi_data.acc_X.axi_short,
                six_axi_data.acc_Y.axi_short,
                six_axi_data.acc_Z.axi_short
        );
        Delay(400);
    }
    
    printf("\r\n***Please tap the device\r\n\r\n");
    Delay(1000);
    
    for (i = 0; i < 20; i++) {
        six_axi_data.acc_X.axi_short = get_acc_X();
        six_axi_data.acc_Y.axi_short = get_acc_Y();
        six_axi_data.acc_Z.axi_short = get_acc_Z();
        
        printf("acc_X: %d, acc_Y: %d, acc_Z:%d\r\n",
                six_axi_data.acc_X.axi_short,
                six_axi_data.acc_Y.axi_short,
                six_axi_data.acc_Z.axi_short
        );
        Delay(400);
    }
    
 #endif 
    printf("   %s end.\r\n", __FUNCTION__);
}



/*  Below is for MS5611     */
static void MS5611_Chip_Enable_Pin_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(MS5611_SPI_CS_GPIO_CLK, ENABLE);
    /* Configure GPIO PIN for Lis Chip select */
    GPIO_InitStructure.GPIO_Pin = MS5611_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MS5611_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

    /* Deselect : Chip Select high */
    GPIO_SetBits(MS5611_SPI_CS_GPIO_PORT, MS5611_SPI_CS_PIN);
}

/*  MS56511 Chip disable    */
static void MS5611_chip_unselect(void)
{
    GPIO_SetBits(MS5611_SPI_CS_GPIO_PORT, MS5611_SPI_CS_PIN);
}
/*  MS5611 Chip enable  */
static void MS5611_chip_select(void)
{
    GPIO_ResetBits(MS5611_SPI_CS_GPIO_PORT, MS5611_SPI_CS_PIN);
}

/*  MS5611 Write @value to @reg     */
static uint8_t MS5611_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;

    MS5611_chip_select();
	status=SPI1_ReadWriteByte(reg);
	//SPI1_ReadWriteByte(value);
    MS5611_chip_unselect();

	return(status);
}

/*  MS5611 read one byte from @reg      */
static uint8_t MS5611_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
    
	MS5611_chip_select();

	SPI1_ReadWriteByte(reg); 
	reg_val=SPI1_ReadWriteByte(0xff);
    
    MS5611_chip_unselect();
    
	return(reg_val);
}
/*  Reset MS5611  */
static void MS5611_Reset(void)
{
    MS5611_chip_select();
    delay(10);
    SPI1_ReadWriteByte(MS5611_RST);
    MS5611_chip_unselect();
}

static uint16_t MS5611_Cali_Data[MS5611_CALI_DATA_NUM];

static void MS5611_Read_PROM(void)
{
    uint8_t i, d1, d2;
    
    for (i = 0; i < MS5611_CALI_DATA_NUM; ++i) {
        MS5611_Cali_Data[i] = MS5611_Read_Reg(MS5611_PROM_RD + i);
        MS5611_Cali_Data[i] <<= 8;
        MS5611_Cali_Data[i] |= MS5611_Read_Reg(MS5611_PROM_RD + i + 1);  
    }
}
uint32_t MS5611_Do_Conversion(uint8_t cmd)
{
    uint32_t conversion = 0;
    uint32_t conv1, conv2, conv3;
    
    MS5611_chip_select();
	SPI1_ReadWriteByte(cmd);
    delay(1000);
	//SPI1_ReadWriteByte(value);
    SPI1_ReadWriteByte(0x00);
    conv1 = SPI1_ReadWriteByte(0xff);
    conv2 = SPI1_ReadWriteByte(0xff);
    conv3 = SPI1_ReadWriteByte(0xff);
    MS5611_chip_unselect();
    
    return conversion = conv1 << 16 | conv2 << 8 | conv3; 
}
/*  MS5611 Init*/
static void MS5611_Init(void)
{
    MS5611_Reset();
    Delay(100);
    MS5611_Read_PROM();
    Delay(100);
}

void test_case_ms5611(void)
{

    printf("\r\n***%s is running\r\n", __FUNCTION__);
    MS5611_Chip_Enable_Pin_Init();
    SPI1_Init();
    MS5611_Init();
    
    printf("read data: 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x\r\n",
            MS5611_Cali_Data[0],
            MS5611_Cali_Data[1],
            MS5611_Cali_Data[3],
            MS5611_Cali_Data[4],
            MS5611_Cali_Data[5],
            MS5611_Cali_Data[6]);
    
    printf("conv data = 0x%08x\r\n",MS5611_Do_Conversion(MS5611_D2_OSR_4096));
    
    printf("*** %s end.\r\n", __FUNCTION__);
    

}
