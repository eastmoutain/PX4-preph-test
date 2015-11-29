#ifndef __PREPH_H__
#define __PREPH_H__

#include <stm32f4xx.h>
#define STM32DISCOVERY
#define RGB_I2C
#define MPU6000
#define MS5611

#if defined(STM32DISCOVERY)
/*
*   Mircos for stm32discovery led 
*/
#define RCC_STATUS_LED_GPIO       RCC_AHB1Periph_GPIOD
#define STATUS_LED_PORT           GPIOD
#define STATUS_LED_PIN            GPIO_Pin_13
#define STATUS_LED1_PIN            GPIO_Pin_12
#endif // STM32DISCOVERY

/*  Mcros for TCA62724FMG I2C DEVICE WHICH CONTROL THE RGB LED BLINK   
*/
#if defined(RGB_I2C)
/*  Config STM32 I2C2 PREPHRIAL */
#define     RCC_RGB_I2Cx_GPIO            RCC_AHB1Periph_GPIOB
#define     RCC_RGB_I2Cx                 RCC_APB1Periph_I2C2
#define     RGB_I2Cx_PORT                GPIOB
#define     RGB_I2Cx_SCL                 GPIO_Pin_10
#define     RGB_I2Cx_SDA                 GPIO_Pin_11
#define     I2Cx_SCL_PinSource           GPIO_PinSource10
#define     I2Cx_SDA_PinSource           GPIO_PinSource11
#define     RGB_I2Cx                     I2C2

/*  Mircos for TCA62724FMG  */
#define     RGB_SLAVE_ADDR               0XAA
#define     RGB_SLAVE_WRITE_ADDR         RGB_SLAVE_ADDR
#define     RGB_SLAVE_READ_ADDR          RGB_SLAVE_ADDR | 0X01

#define     RGB_PWM0_ADDR                0X01
#define     RGB_PWM0_ADDR_AI             (RGB_PWM0_ADDR)
#define     RGB_PWM0_ADDR_NO_AI          (RGB_PWM0_ADDR | 0X01 << 7)

#define     RGB_PWM1_ADDR                0X02
#define     RGB_PWM1_ADDR_AI             (RGB_PWM1_ADDR)
#define     RGB_PWM1_ADDR_NO_AI          (RGB_PWM1_ADDR | 0X01 << 7)

#define     RGB_PWM2_ADDR                0X03
#define     RGB_PWM2_ADDR_AI             (RGB_PWM2_ADDR)
#define     RGB_PWM2_ADDR_NO_AI          (RGB_PWM2_ADDR | 0X01 << 7)

#define     RGB_EN_SHDN_ADDR             0X04
#define     RGB_EN_SHDN_ADDR_AI          RGB_EN_SHDN_ADDR
#define     RGB_EN_SHDN_ADDR_NO_AI       (RGB_EN_SHDN_ADDR | 0X01 << 7)

#define     PWM_DUTY15               0X0F
#define     PWM_DUTY14               0X0E
#define     PWM_DUTY13               0X0D
#define     PWM_DUTY12               0X0C
#define     PWM_DUTY11               0x0B
#define     PWM_DUTY10               0X0A
#define     PWM_DUTY9                0X09
#define     PWM_DUTY8                0X08
#define     PWM_DUTY7                0X07
#define     PWM_DUTY6                0X06
#define     PWM_DUTY5                0X05
#define     PWM_DUTY4                0X04
#define     PWM_DUTY3                0X03
#define     PWM_DUTY2                0X02
#define     PWM_DUTY1                0X01
#define     PWM_DUTY0                0X00

#endif // RGB_I2C

#if defined(MPU6000)
/*  Mircos for MPU60XX  */
#define MPU60XX_SPI                       SPI1
#define MPU60XX_SPI_CLK                   RCC_APB2Periph_SPI1

#define MPU60XX_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define MPU60XX_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define MPU60XX_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define MPU60XX_SPI_SCK_SOURCE            GPIO_PinSource5
#define MPU60XX_SPI_SCK_AF                GPIO_AF_SPI1

#define MPU60XX_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.6 */
#define MPU60XX_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define MPU60XX_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define MPU60XX_SPI_MISO_SOURCE           GPIO_PinSource6
#define MPU60XX_SPI_MISO_AF               GPIO_AF_SPI1

#define MPU60XX_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.7 */
#define MPU60XX_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define MPU60XX_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define MPU60XX_SPI_MOSI_SOURCE           GPIO_PinSource7
#define MPU60XX_SPI_MOSI_AF               GPIO_AF_SPI1
// CS 0: SPI Mode 1: I2C Mode
#define MPU60XX_SPI_CS_PIN                GPIO_Pin_2                  /* PC.2 */
#define MPU60XX_SPI_CS_GPIO_PORT          GPIOC                       /* GPIOC */
#define MPU60XX_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOC
 // DARD
#define MPU60XX_SPI_INT1_PIN              GPIO_Pin_15                  /* PD.15 */
#define MPU60XX_SPI_INT1_GPIO_PORT        GPIOD                       /* GPIOD */
#define MPU60XX_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define MPU60XX_SPI_INT1_EXTI_LINE        EXTI_Line15
#define MPU60XX_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOD
#define MPU60XX_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource15
#define MPU60XX_SPI_INT1_EXTI_IRQn        EXTI15_IRQn 

#if 0
#define MPU60XX_SPI_INT2_PIN              GPIO_Pin_1                  /* PE.01 */
#define MPU60XX_SPI_INT2_GPIO_PORT        GPIOE                       /* GPIOE */
#define MPU60XX_SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define MPU60XX_SPI_INT2_EXTI_LINE        EXTI_Line1
#define MPU60XX_SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define MPU60XX_SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource1
#define MPU60XX_SPI_INT2_EXTI_IRQn        EXTI1_IRQn 
#endif


// MPU60XX Registers
// MPU6000 registers
#define MPUREG_AUX_VDDIO  0x01

#define MPUREG_XG_OFFS_TC 0x00
#define MPUREG_YG_OFFS_TC 0x01
#define MPUREG_ZG_OFFS_TC 0x02
#define MPUREG_X_FINE_GAIN 0x03
#define MPUREG_Y_FINE_GAIN 0x04
#define MPUREG_Z_FINE_GAIN 0x05
#define MPUREG_XA_OFFS_H 0x06
#define MPUREG_XA_OFFS_L 0x07
#define MPUREG_YA_OFFS_H 0x08
#define MPUREG_YA_OFFS_L 0x09
#define MPUREG_ZA_OFFS_H 0x0A
#define MPUREG_ZA_OFFS_L 0x0B
#define MPUREG_PRODUCT_ID 0x0C
#define MPUREG_XG_OFFS_USRH 0x13
#define MPUREG_XG_OFFS_USRL 0x14
#define MPUREG_YG_OFFS_USRH 0x15
#define MPUREG_YG_OFFS_USRL 0x16
#define MPUREG_ZG_OFFS_USRH 0x17
#define MPUREG_ZG_OFFS_USRL 0x18
#define MPUREG_SMPLRT_DIV 0x19
#define MPUREG_CONFIG 0x1A
#define MPUREG_GYRO_CONFIG 0x1B
#define MPUREG_ACCEL_CONFIG 0x1C
#define MPUREG_INT_PIN_CFG 0x37
#define MPUREG_INT_ENABLE 0x38
#define MPUREG_ACCEL_XOUT_H 0x3B
#define MPUREG_ACCEL_XOUT_L 0x3C
#define MPUREG_ACCEL_YOUT_H 0x3D
#define MPUREG_ACCEL_YOUT_L 0x3E
#define MPUREG_ACCEL_ZOUT_H 0x3F
#define MPUREG_ACCEL_ZOUT_L 0x40
#define MPUREG_TEMP_OUT_H 0x41
#define MPUREG_TEMP_OUT_L 0x42
#define MPUREG_GYRO_XOUT_H 0x43
#define MPUREG_GYRO_XOUT_L 0x44
#define MPUREG_GYRO_YOUT_H 0x45
#define MPUREG_GYRO_YOUT_L 0x46
#define MPUREG_GYRO_ZOUT_H 0x47
#define MPUREG_GYRO_ZOUT_L 0x48
#define MPUREG_USER_CTRL 0x6A
#define MPUREG_PWR_MGMT_1 0x6B
#define MPUREG_PWR_MGMT_2 0x6C
#define MPUREG_BANK_SEL 0x6D
#define MPUREG_MEM_START_ADDR 0x6E
#define MPUREG_MEM_R_W 0x6F
#define MPUREG_DMP_CFG_1 0x70
#define MPUREG_DMP_CFG_2 0x71
#define MPUREG_FIFO_COUNTH 0x72
#define MPUREG_FIFO_COUNTL 0x73
#define MPUREG_FIFO_R_W 0x74
#define MPUREG_WHOAMI 0x75

// Configuration bits MPU6000
#define BIT_SLEEP                                       0x40
#define BIT_H_RESET                             0x80
#define BITS_CLKSEL                             0x07
#define MPU_CLK_SEL_PLLGYROX            0x01
#define MPU_CLK_SEL_PLLGYROZ            0x03
#define MPU_EXT_SYNC_GYROX                      0x02
#define BITS_FS_250DPS              0x00
#define BITS_FS_500DPS              0x08
#define BITS_FS_1000DPS             0x10
#define BITS_FS_2000DPS             0x18
#define BITS_FS_2G                  0x00
#define BITS_FS_4G                  0x08
#define BITS_FS_8G                  0x10
#define BITS_FS_16G                 0x18
#define BITS_FS_MASK                0x18
#define BITS_DLPF_CFG_256HZ_NOLPF2  0x00
#define BITS_DLPF_CFG_188HZ         0x01
#define BITS_DLPF_CFG_98HZ          0x02
#define BITS_DLPF_CFG_42HZ          0x03
#define BITS_DLPF_CFG_20HZ          0x04
#define BITS_DLPF_CFG_10HZ          0x05
#define BITS_DLPF_CFG_5HZ           0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF  0x07
#define BITS_DLPF_CFG_MASK          0x07
//#define BIT_INT_ANYRD_2CLEAR        0x10
//#define BIT_RAW_RDY_EN              0x01
#define BIT_I2C_IF_DIS              0x10

// Register 55 - INT Pin / Bypass Enable Configuration (INT_PIN_CFG)
#define BIT_INT_LEVEL        0x80
#define BIT_INT_OPEN         0x40
#define BIT_LATCH_INT_EN     0x20
#define BIT_INT_RD_CLEAR     0x10
#define BIT_FSYNC_INT_LEVEL  0x08
#define BIT_FSYNC_INT_EN     0x04
#define BIT_I2C_BYPASS_EN    0x02
#define BIT_CLKOUT_EN        0x01

// Register 56 - Interrupt Enable (INT_ENABLE)
#define BIT_FF_EN            0x80
#define BIT_MOT_EN           0x40
#define BIT_ZMOT_EN          0x20
#define BIT_FIFO_OFLOW_EN    0x10
#define BIT_I2C_MST_INT_EN   0x08
#define BIT_DATA_RDY_EN      0x01

// Register 58 - Interrupt Status (INT_STATUS)
#define BIT_FF_INT           0x80
#define BIT_MOT_INT          0x40
#define BIT_ZMOT_INT         0x20
#define BIT_FIFO_OFLOW_INT   0x10
#define BIT_I2C_MST_INT      0x08
#define BIT_DATA_RDY_INT     0x01


// DMP output rate constants
#define MPU6000_200HZ 0    // default value
#define MPU6000_100HZ 1
#define MPU6000_66HZ 2
#define MPU6000_50HZ 3

#endif // MPU6000

/*  Mcros for MS561101BA    */
#if defined(MS5611)
// CS 0: SPI Mode 1: I2C Mode
#define MS5611_SPI_CS_PIN                GPIO_Pin_7                  /* PC.7 */
#define MS5611_SPI_CS_GPIO_PORT          GPIOD                       /* GPIOD */
#define MS5611_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOD

#define  MS5611_D1 `            0x40 
#define  MS5611_D2              0x50 
#define  MS5611_RST             0x1E 
#define  MS5611_D1_OSR_4096     0x48 
#define  MS5611_D2_OSR_4096     0x58 
#define  MS5611_ADC_RD          0x00 
#define  MS5611_PROM_RD         0xA0 
#define  MS5611_PROM_CRC        0xAE 

#define MS5611_CALI_DATA_NUM    7

#endif //MS5611


#endif 
