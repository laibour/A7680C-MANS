#include <string.h>
#include "A7680C_LANS_GPIO.h"
#include "simcom_os.h"
#include "simcom_system.h"
#include "simcom_debug.h"
#include "stdio.h"
#include "simcom_uart.h"
#include "simcom_network.h"
#include "my_task.h"

void Task1Processer(void* argv)
{
	UINT8 cnt = 0, led_status = 0;

	printf("Task1Processer is running...\r\n");
	
 	SC_GPIOConfiguration pinConfig = {.pinDir = SC_GPIO_OUT_PIN,
 									  .initLv = 1,
									  .pinPull = SC_GPIO_PULLUP_ENABLE,
									  .pinEd = SC_GPIO_NO_EDGE,
                					  .isr = NULL,
                					  .wu = NULL };

    int ret = sAPI_GpioConfig(SC_MODULE_GPIO_09, pinConfig);
    if (ret)
   		printf("gpio [%d] cfg fail.", SC_MODULE_GPIO_09);
    
	while (1)
	{
		led_status ^= 1;
		
		sAPI_GpioSetValue(SC_MODULE_GPIO_09, led_status);
		
		printf("Task1Processer is running %d.\r\n", cnt++);

		sAPI_TaskSleep(100);
	}
}

extern sTaskRef Task1Ref;
extern sTaskRef Task2Ref;

void Task2Processer(void* argv)
{
	printf("Task2Processer is running...\r\n");
	
	sAPI_TaskSleep(1000);
	
	sAPI_TaskSuspend(Task1Ref);
	
	if((SC_Uart_Return_Code)sAPI_SystemSleepSet(SC_SYSTEM_SLEEP_ENABLE) == SC_UART_RETURN_CODE_ERROR)
    {
        printf("Set sleep mode error.\r\n");
    }
}

