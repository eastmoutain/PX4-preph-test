/******************************************************************
**	  ����407�����壨V1.0��
**	  SysTick�����ļ�
**
**	  ��    ̳��bbs.openmcu.com
**	  ��    ����www.openmcu.com
**	  ��    �䣺support@openmcu.com
**
**    ��    ����V1.0
**	  ��    �ߣ�FXL
**	  �������:	2013.1.4
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
*�������ƣ�void SysTick_Init(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����SysTick��ʼ��  �����ʼ��ʧ�ܣ���ͣ��WHILE��ѭ��
*******************************************************************************************/
void SysTick_Init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))    //1ms
    { 
        while (1); //��ʼ��ʧ��
    }
}

/******************************************************************************************
*�������ƣ�void Delay(__IO uint32_t nTime)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵�������ⲿ���õ���ʱ����
*******************************************************************************************/
void Delay(__IO uint32_t nTime)
{ 
    TimingDelay = nTime;
    
    while(TimingDelay != 0);
}

/******************************************************************************************
*�������ƣ�void TimingDelay_Decrement(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����SysTick�жϵ��ú���
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



