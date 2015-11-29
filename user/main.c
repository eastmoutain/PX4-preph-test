#include "stm32f4xx.h"
#include "uart.h" 
#include "nvic.h"
#include "exti.h"
#include "SysTick.h"
#include "stdio.h"

enum{
    TEST_START = 0,
    TEST_RGB = TEST_START,
    TEST_MPU6000,
    TESE_MS5611,
    TEST_SD_CARD,
    TEST_END
};

char *test_case[] = {
	"   Case 1: RGB LED test\r\n",
	"   Case 2: MPU6000 test\r\n",
    "   Case 3: MS5611 test\r\n",
    "   Case 4 SD-Card test\r\n"
};

extern void LedBlink_Init(void);
extern void test_case_rgb(void);
extern void test_case_mpu6000(void);
extern void test_case_ms5611(void);
extern void test_case_sd_card(void);

#define TEST_CASE_NUM  \
        (sizeof(test_case) / sizeof(test_case[0]))

typedef void (*test_func)(void);

test_func test_funcs[] = {
    test_case_rgb,
    test_case_mpu6000,
    test_case_ms5611,
    test_case_sd_card
};

void print_test_cases(void)
{
	int loop_cnt = 0;
	
	printf("===============================\r\n");
	for (loop_cnt = 0; loop_cnt < TEST_CASE_NUM; ++loop_cnt) {
		printf("%s", test_case[loop_cnt]);
	}
	printf("\r\n");
}
	

int main(void)
{
    int order;
	SysTick_Init();
	NVIC_Config();
    LedBlink_Init();
	uart_init();
	
	
	printf("###########################################################################\r\n ");
	printf("Using USART2 As serial port, Baurd: 57600, TX: GPIO PD5, RX: GPIO PD6\r\n\r\n");
	printf("Please select testcase to run...\r\n");
	
	print_test_cases();
	
	while(1) {
		
		Delay(200);
		for (order = TEST_START; order < TEST_END; order++) {
//			c = GetChar();
//			if (c) {
//				printf("Get char: %c\r\n", c);	
//			}
//			c = c - '0';
            printf("\r\n=======================================================\r\n");
			test_funcs[order]();
			//break;
		}
		while(1);
	}

}
