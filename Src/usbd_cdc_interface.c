/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Src/usbd_cdc_interface.c
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    12-May-2017
  * @brief   Source file for USBD CDC interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright � 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "stm32f1xx_hal.h"
#include "usbd_core.h"
#include "stm32f1xx_hal_pcd.h"
#include "usbd_desc.h"
#include "usbd_cdc.h" 
#include "usbd_cdc_interface.h"

#include "terminal.h"
#include "termgpio.h"
#include "blue_pill.h"
#include "main.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_CDC 
  * @brief usbd core module
  * @{
  */

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
#define APP_RX_DATA_SIZE  1024
#define APP_TX_DATA_SIZE  1024

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */

uint8_t UserRxBuffer[APP_RX_DATA_SIZE]; /* Received Data over USB are stored in 
                                         * this buffer */
uint8_t UserTxBuffer[APP_TX_DATA_SIZE]; /* Received Data over UART (CDC
                                         * interface) are stored in this buffer 
                                         */
uint32_t BuffLength;
uint32_t UserTxBufPtrIn = 0;    /* Increment this pointer or roll it back to
                                 * start address when data are received over
                                 * USART */
uint32_t UserTxBufPtrOut = 0;   /* Increment this pointer or roll it back to
                                 * start address when data are sent over USB */

/* UART handler declaration */
UART_HandleTypeDef UartHandle;
/* TIM handler declaration */
TIM_HandleTypeDef TimHandle;
/* USB handler declaration */
extern USBD_HandleTypeDef USBD_Device;

/* Private function prototypes ----------------------------------------------- */
static int8_t CDC_Itf_Init(void);
static int8_t CDC_Itf_DeInit(void);
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t * pbuf, uint16_t length);
static int8_t CDC_Itf_Receive(uint8_t * pbuf, uint32_t * Len);

//static void ComPort_Config(void);
static void TIM_Config(void);
static void CDC_tx_timer_init(void);
static void CDC_tx(const uint8_t* buff, uint32_t len);

USBD_CDC_ItfTypeDef USBD_CDC_fops = {
  CDC_Itf_Init,
  CDC_Itf_DeInit,
  CDC_Itf_Control,
  CDC_Itf_Receive
  };

/* Private functions --------------------------------------------------------- */


/**
  * @brief  CDC_Itf_Init
  *         Initializes the CDC media low layer
  * @param  None
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_Init(void)
{

    TERM_init(CDC_tx);
    
    CDC_tx_timer_init();
    /* ## Set Application Buffers ############################################ */
    USBD_CDC_SetTxBuffer(&USBD_Device, UserTxBuffer, 0);
    USBD_CDC_SetRxBuffer(&USBD_Device, UserRxBuffer);

    return (USBD_OK);
}

/**
  * @brief  CDC_Itf_DeInit
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_DeInit(void)
{
     TERM_deinit();
     return (USBD_OK);
}

/**
  * @brief  CDC_Itf_Control
  *         Manage the CDC class requests
  * @param  Cmd: Command code            
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t * pbuf, uint16_t length)
{
    USBD_CDC_LineCodingTypeDef lineCoding;
    switch (cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:
        /* Add your code here */
        break;
        
    case CDC_GET_ENCAPSULATED_RESPONSE:
        /* Add your code here */
        break;
        
    case CDC_SET_COMM_FEATURE:
        /* Add your code here */
        break;
        
    case CDC_GET_COMM_FEATURE:
        /* Add your code here */
        break;
        
    case CDC_CLEAR_COMM_FEATURE:
        /* Add your code here */
        break;
        
    case CDC_SET_LINE_CODING:
        lineCoding.bitrate = (uint32_t) (pbuf[0] | (pbuf[1] << 8) |
                                         (pbuf[2] << 16) | (pbuf[3] << 24));
        lineCoding.format = pbuf[4];
        lineCoding.paritytype = pbuf[5];
        lineCoding.datatype = pbuf[6];
        
        /* Set the new configuration */
        //ComPort_Config();
        TERM_set_config(&lineCoding);
        break;

    case CDC_GET_LINE_CODING:
        TERM_get_config(&lineCoding);
        
        pbuf[0] = (uint8_t) (lineCoding.bitrate);
        pbuf[1] = (uint8_t) (lineCoding.bitrate >> 8);
        pbuf[2] = (uint8_t) (lineCoding.bitrate >> 16);
        pbuf[3] = (uint8_t) (lineCoding.bitrate >> 24);
        pbuf[4] = lineCoding.format;
        pbuf[5] = lineCoding.paritytype;
        pbuf[6] = lineCoding.datatype;
        
        /* Add your code here */
        break;
        
    case CDC_SET_CONTROL_LINE_STATE:
        /* Add your code here */
        break;
        
    case CDC_SEND_BREAK:
        /* Add your code here */
        break;
        
    default:
        break;
    }
    
    return (USBD_OK);
}

/**
  * @brief  TIM period elapsed callback
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
#if 1
    uint8_t *buffptr;
    uint32_t buffsize;

    if (UserTxBufPtrOut != UserTxBufPtrIn)
    {
        if (UserTxBufPtrOut > UserTxBufPtrIn) /* rollback */
        {
            buffsize = APP_RX_DATA_SIZE - UserTxBufPtrOut;
        }
        else
        {
            buffsize = UserTxBufPtrIn - UserTxBufPtrOut;
        }
        
        buffptr = UserTxBuffer + UserTxBufPtrOut;
        
        USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t *) buffptr,
                             buffsize);
        
        if (USBD_CDC_TransmitPacket(&USBD_Device) == USBD_OK)
        {
            //BRD_led_toggle();
            UserTxBufPtrOut += buffsize;
            if (UserTxBufPtrOut >= APP_RX_DATA_SIZE)
            {
                UserTxBufPtrOut = 0;
            }
            BRD_led_off();
        }
        //BRD_led_off();
    }
#else
    memset(&UserTxBuffer, 'e', 5);
    UserTxBuffer[6] = '\n';
    UserTxBuffer[7] = '\r';
    USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t *) UserTxBuffer,
                             8);
    if (USBD_CDC_TransmitPacket(&USBD_Device) == USBD_OK)
    {
        //BRD_led_off();
    }
#endif
}


/**
  * @brief  CDC_Itf_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  * @param  Buf: Buffer of data to be transmitted
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_Receive(uint8_t * buf, uint32_t * len)
{
    BRD_led_on();
    //LED_Toggle();
    // echo test
    CDC_tx(buf, *len);

    USBD_CDC_SetRxBuffer(&USBD_Device, UserRxBuffer);
    USBD_CDC_ReceivePacket(&USBD_Device);
  
    return (USBD_OK);
}
/**
  * @brief  TIM_Config: Configure TIMx timer
  * @param  None.
  * @retval None.
  */
static void TIM_Config(void)
{
  /* Set TIMx instance */
  TimHandle.Instance = TIMx;

  /* Initialize TIM3 peripheral as follows: + Period = 10000 - 1 + Prescaler =
   * ((SystemCoreClock/2)/10000) - 1 + ClockDivision = 0 + Counter direction =
   * Up */
  TimHandle.Init.Period = (CDC_POLLING_INTERVAL * 1000) - 1;
  TimHandle.Init.Prescaler = 84 - 1;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

static void CDC_tx_timer_init(){
   

    TIMx_CLK_ENABLE();

    /* ##- Configure the NVIC for TIMx ######################################## */
    /* Set Interrupt Group Priority */
    HAL_NVIC_SetPriority(TIMx_IRQn, 5, 0);
    
    /* Enable the TIMx global Interrupt */
    HAL_NVIC_EnableIRQ(TIMx_IRQn);
    /* Configure the TIM Base generation ################################# */
    TIM_Config();
    
    /* ##-4- Start the TIM Base generation in interrupt mode #################### */
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }
}

static void CDC_tx(const uint8_t* buff, uint32_t len)
{
    uint8_t *buffptr;
    uint32_t buffsize;

    buffptr = UserTxBuffer + UserTxBufPtrIn;
     if (UserTxBufPtrOut > UserTxBufPtrIn) /* rollback */
     {
         buffsize = UserTxBufPtrOut - UserTxBufPtrIn - 1;
         if(len > buffsize)
         {
             memcpy(buffptr, buff, buffsize);
             UserTxBufPtrIn +=buffsize;
         }
         else
         {
             memcpy(buffptr, buff, len);
             UserTxBufPtrIn += len;
         }   
     }
     else
     {
         if (UserTxBufPtrIn + len >= APP_RX_DATA_SIZE)
         {
             buffsize = APP_RX_DATA_SIZE - UserTxBufPtrIn - 1;
             memcpy(buffptr, buff, buffsize);
             UserTxBufPtrIn += len;

             memcpy(UserTxBuffer,
                    buff + buffsize,
                    len - buffsize);
             
             UserTxBufPtrIn = len - buffsize;
         }
         else
         {
             memcpy(buffptr, buff, len);
             UserTxBufPtrIn += len;
         }
     }
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef * UartHandle)
{
  /* Transfer error occurred in reception and/or transmission process */
  Error_Handler();
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
