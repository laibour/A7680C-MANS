/**
  ******************************************************************************
  * @file    demo_helloworld.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of helloworld.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */
 
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "A7680C_LANS_GPIO.h"
#include "simcom_os.h"
#include "simcom_system.h"
#include "simcom_debug.h"
#include "stdio.h"
#include "simcom_uart.h"
#include "simcom_network.h"
#include "my_task.h"

sTaskRef Task1Ref;
static UINT8 Task1ProcesserStack[1024];

sTaskRef Task2Ref;
static UINT8 Task2ProcesserStack[1024];

/**
  * @brief  helloworld task initial
  * @param  void
  * @note   
  * @retval void
  */
void sAPP_HelloWorldDemo(void)
{
    SC_STATUS status = SC_SUCCESS;

    status = sAPI_TaskCreate(&Task1Ref, Task1ProcesserStack, 1024, 150, "Task1Processer", Task1Processer, (void *)0);
    if(SC_SUCCESS != status)
    {
        sAPI_Debug("Task1 create fail,status = [%d]",status);
    }

	status = sAPI_TaskCreate(&Task2Ref, Task2ProcesserStack, 1024, 151, "Task2Processer", Task2Processer, (void *)0);
    if(SC_SUCCESS != status)
    {
        sAPI_Debug("Task2 create fail,status = [%d]",status);
    }
}

#if 0
/**
  * @brief  simcom printf
  * @param  pointer *format
  * @note   The message will be output by sAPI_Debug and captured by CATSTUDIO tool.
  * @retval void
  */
void simcom_printf(const char *format,...){
    char tmpstr[200];

    va_list args;

    memset(tmpstr,0,sizeof(tmpstr));

    va_start(args,format);
    sAPI_Vsnprintf(tmpstr,sizeof(tmpstr),format,args);
    va_end(args);

	sAPI_UartWrite(SC_UART1, (UINT8 *)tmpstr, strlen(tmpstr));
}
#endif

