#include <stm32f4xx.h>
#include "preph.h"
#include "i2c.h"
#include <stdio.h>
#include "SysTick.h"

static void init_I2C(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	// enable APB1 peripheral clock for I2C2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	// enable clock for SCL and SDA pins
	RCC_AHB1PeriphClockCmd(RCC_RGB_I2Cx_GPIO, ENABLE);
    
    // Connect I2C1 pins to AF  
	GPIO_PinAFConfig(RGB_I2Cx_PORT, I2Cx_SCL_PinSource, GPIO_AF_I2C2);	// SCL
	GPIO_PinAFConfig(RGB_I2Cx_PORT, I2Cx_SDA_PinSource, GPIO_AF_I2C2); // SDA
	
	/* setup SCL and SDA pins
	 * You can connect I2C1 to two different
	 * pairs of pins:
	 * 1. SCL on PB6 and SDA on PB7 
	 * 2. SCL on PB8 and SDA on PB9
	 */
	GPIO_InitStruct.GPIO_Pin = RGB_I2Cx_SCL | RGB_I2Cx_SDA; // we are going to use PB6 and PB7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			// set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// enable pull up resistors
	GPIO_Init(RGB_I2Cx_PORT, &GPIO_InitStruct);					// init GPIOB
	
	// configure I2C2 
    I2C_DeInit(RGB_I2Cx);
	I2C_InitStruct.I2C_ClockSpeed = 100000; 		// 100kHz
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;			// I2C mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	// 50% duty cycle --> standard
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;			// own address, not relevant in master mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;		// disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
	
    // enable I2C1
	I2C_Cmd(RGB_I2Cx, ENABLE);
    I2C_Init(RGB_I2Cx, &I2C_InitStruct);				// init I2C1
	
}

void test_case_rgb(void)
{
    unsigned char data[2] = {0,0};
    int i = 0;
    
    printf("\r\n*** %s strat running\r\n", __FUNCTION__);
    
    init_I2C();
    
    I2C_start(RGB_I2Cx, RGB_SLAVE_ADDR, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
    // Write for PWM0, ADDR and DATA
    I2C_write(RGB_I2Cx, RGB_PWM0_ADDR_AI);
    I2C_write(RGB_I2Cx, PWM_DUTY0);
    // Write for PWM1, ADDR and DATA
    I2C_write(RGB_I2Cx, RGB_PWM1_ADDR_AI);
    I2C_write(RGB_I2Cx, PWM_DUTY0);
    // Write for PWM2, ADDR and DATA
    I2C_write(RGB_I2Cx, RGB_PWM2_ADDR_AI);
    I2C_write(RGB_I2Cx, PWM_DUTY1);
    // Write for PWM0, ADDR and DATA
    I2C_write(RGB_I2Cx, RGB_EN_SHDN_ADDR_AI);
    I2C_write(RGB_I2Cx, 0X03);
    I2C_stop(RGB_I2Cx); // stop the transmission
    
    I2C_start(RGB_I2Cx, RGB_SLAVE_ADDR, I2C_Direction_Receiver); // start a transmission in Master transmitter mode
    data[0] = I2C_read_ack(RGB_I2Cx);
    data[1] = I2C_read_nack(RGB_I2Cx);
    I2C_stop(RGB_I2Cx); // stop the transmission
    
    if (0x30 == data[0] && 0x20 == data[1]) {
        printf("  I2C slave works okay, please note the LED color changement\r\n");
    }
    
    for (i = PWM_DUTY0; i <= PWM_DUTY15; ++i) {
        I2C_start(RGB_I2Cx, RGB_SLAVE_ADDR, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
        // Write for PWM0, ADDR and DATA
        I2C_write(RGB_I2Cx, RGB_PWM0_ADDR_AI);
        I2C_write(RGB_I2Cx, i);
        I2C_stop(RGB_I2Cx); // stop the transmission
        Delay(1000);
    }
    I2C_start(RGB_I2Cx, RGB_SLAVE_ADDR, I2C_Direction_Transmitter);
    I2C_write(RGB_I2Cx, RGB_PWM0_ADDR_AI);
    I2C_write(RGB_I2Cx, PWM_DUTY0);
    I2C_stop(RGB_I2Cx); // stop the transmission
    
    for (i = PWM_DUTY0; i <= PWM_DUTY15; ++i) {
        I2C_start(RGB_I2Cx, RGB_SLAVE_ADDR, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
        // Write for PWM0, ADDR and DATA
        I2C_write(RGB_I2Cx, RGB_PWM1_ADDR_AI);
        I2C_write(RGB_I2Cx, i);
        I2C_stop(RGB_I2Cx); // stop the transmission
        Delay(1000);
    }
    
    I2C_start(RGB_I2Cx, RGB_SLAVE_ADDR, I2C_Direction_Transmitter);
    I2C_write(RGB_I2Cx, RGB_PWM1_ADDR_AI);
    I2C_write(RGB_I2Cx, PWM_DUTY0);
    I2C_stop(RGB_I2Cx); // stop the transmission
    
    for (i = PWM_DUTY0; i <= PWM_DUTY15; ++i) {
        I2C_start(RGB_I2Cx, RGB_SLAVE_ADDR, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
        // Write for PWM0, ADDR and DATA
        I2C_write(RGB_I2Cx, RGB_PWM2_ADDR_AI);
        I2C_write(RGB_I2Cx, i);
        I2C_stop(RGB_I2Cx); // stop the transmission
        Delay(1000);
    }
    
    I2C_start(RGB_I2Cx, RGB_SLAVE_ADDR, I2C_Direction_Transmitter);
    I2C_write(RGB_I2Cx, RGB_PWM2_ADDR_AI);
    I2C_write(RGB_I2Cx, PWM_DUTY0);
    I2C_stop(RGB_I2Cx); // stop the transmission
    
    
    printf("*** %s end.\r\n", __FUNCTION__);
}


#if defined (STM32DISCOVERY)
void LedBlink_Init(void)
{	GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_AHB1PeriphClockCmd(RCC_STATUS_LED_GPIO, ENABLE);
    
    
	GPIO_InitStruct.GPIO_Pin = STATUS_LED_PIN | STATUS_LED1_PIN; // we are going to use PB6 and PB7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;			// set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// enable pull up resistors
    
	GPIO_Init(STATUS_LED_PORT, &GPIO_InitStruct);
    GPIO_WriteBit(STATUS_LED_PORT, STATUS_LED_PIN, 0x01);
    GPIO_WriteBit(STATUS_LED_PORT, STATUS_LED1_PIN, 0x01);
    
}

static void LedToggle(void)
{
    uint8_t status = GPIO_ReadOutputDataBit(STATUS_LED_PORT, STATUS_LED_PIN);
    
    GPIO_WriteBit(STATUS_LED_PORT, STATUS_LED_PIN, !status);
}
void LedBlink(void *arg)
{
    LedToggle();
}

static void Led1Toggle(void)
{
    uint8_t status = GPIO_ReadOutputDataBit(STATUS_LED_PORT, STATUS_LED1_PIN);
    
    GPIO_WriteBit(STATUS_LED_PORT, STATUS_LED1_PIN, !status);
}
void Led1Blink(void *arg)
{
    Led1Toggle();
}

#endif
