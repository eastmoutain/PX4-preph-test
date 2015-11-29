/******************************************************************
**	  红龙407开发板（V1.0）
**	  SysTick配置文件
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.0
**	  作    者：FXL
**	  完成日期:	2013.1.4
********************************************************************/
#include <stdlib.h>
#include "stm32f4xx.h"
#include "SysTick.h"

__IO uint32_t TimingDelay;


/***************************************************************************
*   For led blinks event
****************************************************************************/
//#ifdef STM32DISCOVERY
extern void LedBlink(void *);
extern void Led1Blink(void *);
//#endif

typedef struct {
    // periodic time
    uint32_t period;
    // if repeat < 0, then repeat it infinitely. 
    int32_t repeat;
    uint32_t cnt;
    void *arg;
    void (*cb)(void *);
} Event_t;

Event_t event[] = {
    {
        500, // period
        -1,
        0,
        NULL,
        LedBlink
    },
    {
        50,
        -1, 
        0,
        NULL,
        Led1Blink
    },
    NULL
};

#define EVENT_NUM  (sizeof(event) / sizeof(event[0]))





/******************************************************************************************
*函数名称：void SysTick_Init(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：SysTick初始化  如果初始化失败，会停入WHILE死循环
*******************************************************************************************/
void SysTick_Init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))    //1ms
    { 
        while (1); //初始化失败
    }
}

/******************************************************************************************
*函数名称：void Delay(__IO uint32_t nTime)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：供外部调用的延时函数
*******************************************************************************************/
void Delay(__IO uint32_t nTime)
{ 
    TimingDelay = nTime;
    
    while(TimingDelay != 0);
}

/******************************************************************************************
*函数名称：void TimingDelay_Decrement(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：SysTick中断调用函数
*******************************************************************************************/
void TimingDelay_Decrement(void)
{
    int i;
    Event_t *event_tmp = NULL;
    
    if (TimingDelay != 0x00)
    { 
        TimingDelay--;
    }
    
    for (i = 0; i < EVENT_NUM; ++i) {
        event_tmp = &event[i];
        if ((event_tmp->cb != NULL) && (++event_tmp->cnt == event_tmp->period)) {
                
            if (event_tmp->repeat-- > 0) {
                --event_tmp->repeat;
                event_tmp->cb(event_tmp->arg);
            } else if (event_tmp->repeat < 0) {
                event_tmp->cb(event_tmp->arg);
            }
            event_tmp->cnt = 0;
        }
    }
}



