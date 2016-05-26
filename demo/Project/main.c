#include "sys.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"

#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_init.h"
#include "MainTask.h"
#include "touch.h"

static void vTaskGUI(void *pvParameters);
void vTaskTaskLED0(void *pvParameters);
void vTaskTaskLED1(void *pvParameters);
void vTaskTaskKEY(void *pvParameters);
    
int main(void)
{
    bsp_Init();//硬件初始化
    printf("硬件初始化完成\r\n");
    
    xTaskCreate(    vTaskTaskKEY,   /* 任务函数  */
                    "vTaskTaskKEY",     /* 任务名    */
                    configMINIMAL_STACK_SIZE,               /* stack大小，单位word，也就是4字节 */
                    NULL,              /* 任务参数  */
                    3,                 /* 任务优先级*/
                    NULL );            /* 任务句柄  */    
    
    xTaskCreate(    vTaskTaskLED0,   /* 任务函数  */
                    "vTaskTaskLED0",     /* 任务名    */
                    configMINIMAL_STACK_SIZE,               /* stack大小，单位word，也就是4字节 */
                    NULL,              /* 任务参数  */
                    2,                 /* 任务优先级*/
                    NULL );            /* 任务句柄  */

    xTaskCreate(    vTaskTaskLED1,   /* 任务函数  */
                    "vTaskTaskLED1",     /* 任务名    */
                    configMINIMAL_STACK_SIZE,               /* stack大小，单位word，也就是4字节 */
                    NULL,              /* 任务参数  */
                    2,                 /* 任务优先级*/
                    NULL );            /* 任务句柄  */
    
	xTaskCreate(    vTaskGUI,          /* 任务函数  */
                    "vTaskGUI",        /* 任务名    */
                    configMINIMAL_STACK_SIZE*30,               /* stack大小，单位word，也就是4字节 */
                    NULL,              /* 任务参数  */
                    1,                 /* 任务优先级*/
                    NULL );            /* 任务句柄  */
                    
    vTaskStartScheduler();
    
    while(1);
}

/*
*********************************************************************************************************
*	函 数 名: vTaskGUI
*	功能说明: emWin任务
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1   (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
static void vTaskGUI(void *pvParameters)
{
	while (1) 
	{
		MainTask();
	}
}

void vTaskTaskLED0(void *pvParameters)
{
    while(1)
    {
        LED0 = !LED0;
        vTaskDelay(500);
    }
}

void vTaskTaskLED1(void *pvParameters)
{
    while(1)
    {
        LED1 = !LED1;
        vTaskDelay(250);
    }
}

void vTaskTaskKEY(void *pvParameters)
{
    while(1)
    {
        GUI_TOUCH_Exec();
//        tp_dev.scan(0);
        vTaskDelay(1);
    }
}

//栈溢出回调函数
//void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName)
//{
//	printf("线程：%s 堆栈溢出\r\n",pcTaskName);
//}

void HardFault_Handler(void)
{
	printf("HardFault_Handler!!!硬件错误!!!\r\n");
}
