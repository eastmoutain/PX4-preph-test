

#include <stdio.h>
#include "stm32f4xx.h"
#include "sdio_sdcard.h"

static char buf[512];

static void memset_u8(char *p, uint32_t num)
{
    while(num--) {
        *p++ = 0;
    }
}

void test_case_sd_card(void)
{
    uint32_t i;
    
    printf("\r\n*** %s strat running\r\n", __FUNCTION__);
    SD_Init();
    show_SD_Card_Info();
    
//    memset_u8(buf, 512);
//    if (SD_ReadDisk(buf, 0, 1) == 0) {
//        for (i = 0; i < 512; ++i) {
//            printf("0x%02x\t", buf[i]);
//        }
//        printf("\r\n");
//    }
//    
    printf("*** %s end.\r\n", __FUNCTION__);
}
