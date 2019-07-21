
#include "stm32f1xx_hal.h"
#include "termgpio.h"
#include "gpio.h"
#include "main.h"


#define TIM_COUNTER_CLOCK 10000 

TIM_HandleTypeDef TIM_PWM_handler[4]; // TIM1 TIM2 ....

static void GPIO_pwm_config()
{
#define  PERIOD_VALUE       (uint32_t)(700 - 1)  /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       (uint32_t)(PERIOD_VALUE*37.5/100) /* Capture Compare 2 Value  */
#define  PULSE3_VALUE       (uint32_t)(PERIOD_VALUE/4)        /* Capture Compare 3 Value  */
#define  PULSE4_VALUE       (uint32_t)(PERIOD_VALUE*12.5/100) /* Capture Compare 4 Value  */

#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL1        GPIOA
#define TIMx_GPIO_PORT_CHANNEL2        GPIOA
#define TIMx_GPIO_PORT_CHANNEL3        GPIOA
#define TIMx_GPIO_PORT_CHANNEL4        GPIOA
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_0
#define TIMx_GPIO_PIN_CHANNEL2         GPIO_PIN_1
#define TIMx_GPIO_PIN_CHANNEL3         GPIO_PIN_2
#define TIMx_GPIO_PIN_CHANNEL4         GPIO_PIN_3

    
    /* Timer handler declaration */
    TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
    TIM_OC_InitTypeDef sConfig;

/* Counter Prescaler value */
    uint32_t uhPrescalerValue = 0;

    GPIO_InitTypeDef   GPIO_InitStruct;
   
    __HAL_RCC_TIM2_CLK_ENABLE();
    //HAL_TIM_ConfigClockSource();

    /* Common configuration for all channels */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL1;
    HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL2;
    HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL2, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL3;
    HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL3, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL4;
    HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL4, &GPIO_InitStruct);




 /* Compute the prescaler value to have TIM2 counter clock equal to 100000 Hz */
  uhPrescalerValue = (uint32_t)(SystemCoreClock / 100000) - 1;
  
  /*##-1- Configure the TIM peripheral #######################################*/
  /* -----------------------------------------------------------------------
  TIM2 Configuration: generate 4 PWM signals with 4 different duty cycles.

    In this example TIM2 input clock (TIM2CLK) is set to APB1 clock (PCLK1) x2,
    since APB1 prescaler is set to 4 (0x100).
       TIM2CLK = PCLK1*2
       PCLK1   = HCLK/2
    => TIM2CLK = PCLK1*2 = (HCLK/2)*2 = HCLK = SystemCoreClock

    To get TIM2 counter clock at 2.1 MHz, the prescaler is computed as follows:
       Prescaler = (TIM2CLK / TIM2 counter clock) - 1
       Prescaler = ((SystemCoreClock) /2.1 MHz) - 1

    To get TIM2 output clock at 3 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM2 counter clock / TIM2 output clock) - 1
           = 699

    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR + 1)* 100 = 50%
    TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR + 1)* 100 = 37.5%
    TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR + 1)* 100 = 25%
    TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR + 1)* 100 = 12.5%

    Note:
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f1xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
  ----------------------------------------------------------------------- */

  /* Initialize TIMx peripheral as follows:
       + Prescaler = (SystemCoreClock / 2000000) - 1
       + Period = (700 - 1)
       + ClockDivision = 0
       + Counter direction = Up
  */

  /*
  *    counter_clock = 100 kHz
  *   output_clock = 50 Hz
  *   ARR = (counter_clock / output_clock) - 1 = 2000-1 = 1999
  *  duty = 50%
  *  CCR = ARR * % / 100 = 1999*0.5 = 999
  *    
  */
  
  TimHandle.Instance = TIM2;

  TimHandle.Init.Prescaler         = uhPrescalerValue;
  TimHandle.Init.Period            = 1999;//PERIOD_VALUE;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
      /* Initialization Error */
      Error_Handler();
  }
  
  /*##-2- Configure the PWM channels #########################################*/
  /* Common configuration for all channels */
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  
  /* Set the pulse value for channel 1 */
  sConfig.Pulse = 999;//PULSE1_VALUE;
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
      /* Configuration Error */
      Error_Handler();
  }
  
  /* Set the pulse value for channel 2 */
  sConfig.Pulse = 333;//PULSE2_VALUE;
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
      /* Configuration Error */
      Error_Handler();
  }
  
  
  /* Set the pulse value for channel 3 */
  sConfig.Pulse = 1700;//PULSE3_VALUE;
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Set the pulse value for channel 4 */
  sConfig.Pulse = 1999;//PULSE4_VALUE;
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
    
  /*##-3- Start PWM signals generation #######################################*/
  /* Start channel 1 */
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  /* Start channel 2 */
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  /* Start channel 3 */
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    /* PWM generation Error */
    Error_Handler();
  }

   /* Start channel 4 */
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4) != HAL_OK)
  {
    /* PWM generation Error */
    Error_Handler();
  }

  
}


static void GPIO_set_gpio_mode(GPIO_TypeDef *port, int gpio, bool level)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = gpio;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
    if(level)
        HAL_GPIO_WritePin(port, gpio, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(port, gpio, GPIO_PIN_RESET);

}

static void GPIO_set_gpio_pwm_mode(GPIO_TypeDef *port, int gpio)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = gpio;
    HAL_GPIO_Init(port, &GPIO_InitStruct);

}

static int GPIO_set_tim_pwm_mode(TIM_HandleTypeDef *handle, int chan, int duty)
{
    unsigned int period = handle->Init.Period;
    TIM_OC_InitTypeDef sConfig;

    if (HAL_TIM_PWM_Init(handle) != HAL_OK)
    {
        return -1;
    }
    
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  

    sConfig.Pulse = period*duty/100;
    if (HAL_TIM_PWM_ConfigChannel(handle, &sConfig, chan) != HAL_OK)
    {
        /* Configuration Error */
        return -1;
    }
    if (HAL_TIM_PWM_Start(handle, chan) != HAL_OK)
    {
        /* PWM Generation Error */
        return -1;
    }
    return 0;
}

static int GPIO_gpio_convert_to_hal(TERM_gpio_port_info_TYP *info,
                                   GPIO_TypeDef **port,
                                   int* gpio)
{
    switch(info->idx.line)
    {
    case 0:
        *gpio = GPIO_PIN_0;
        break;
    case 1:
        *gpio = GPIO_PIN_1;
        break;
    case 2:
        *gpio = GPIO_PIN_2;
        break;
    case 3:
        *gpio = GPIO_PIN_3;
        break;
    case 4:
        *gpio = GPIO_PIN_4;
        break;
    case 5:
        *gpio = GPIO_PIN_5;
        break;
    case 6:
        *gpio = GPIO_PIN_6;
        break;
    case 7:
        *gpio = GPIO_PIN_7;
        break;
    case 8:
        *gpio = GPIO_PIN_8;
        break;
    case 9:
        *gpio = GPIO_PIN_9;
        break;
    case 10:
        *gpio = GPIO_PIN_10;
        break;
    case 11:
        *gpio = GPIO_PIN_11;
        break;
    case 12:
        *gpio = GPIO_PIN_12;
        break;
    case 13:
        *gpio = GPIO_PIN_13;
        break;
    case 14:
        *gpio = GPIO_PIN_14;
        break;
    case 15:
        *gpio = GPIO_PIN_15;
        break;
    default:
        return -1;
    }

    switch(info->idx.port)
    {
    case 'A':
            *port = GPIOA;
            break;
    case 'B':
            *port = GPIOB;
            break;
    case 'C':
            *port = GPIOC;
            break;
    case 'D':
            *port = GPIOD;
            break;
    default:
        return -1;
    }
    
    return 0;
}

static int GPIO_tim_convert_to_hal(TERM_gpio_tim_pwm_info_TYP *info,
                                   TERM_gpio_port_TYP *idx,
                                   TIM_TypeDef **tim,
                                   int* channel)
{
    switch(info->tim)
    {
    case 1:
        *tim = TIM1;
        break;
    case 2:
        *tim = TIM2;
        break;
    case 3:
        *tim = TIM3;
        break;
    case 4:
        *tim = TIM4;
        break;
    default:
        return -1;
    }
    if((info->channels[0].port == idx->port) &&
       (info->channels[0].line == idx->line))
    {
        *channel = TIM_CHANNEL_1;
    }
    else if((info->channels[1].port == idx->port) &&
       (info->channels[1].line == idx->line))
    {
        *channel = TIM_CHANNEL_2;
    }
    else if((info->channels[2].port == idx->port) &&
       (info->channels[2].line == idx->line))
    {
        *channel = TIM_CHANNEL_3;
    }
    else if((info->channels[3].port == idx->port) &&
            (info->channels[3].line == idx->line))
    {
        *channel = TIM_CHANNEL_4;
    }
    else
    {
        return -1;
    }
    return 0;
}

int GPIO_init(TERM_gpio_port_info_TYP* gpio_table,
              TERM_gpio_tim_pwm_info_TYP *pwm_table)
{
    TERM_gpio_port_info_TYP *gpio_line = gpio_table;
    TERM_gpio_tim_pwm_info_TYP * pwm =pwm_table;
        

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();
        
    while((gpio_line->idx.port != 0) &&
          (gpio_line->idx.line != 0))
    {

        GPIO_set_mode(gpio_line);
        
        gpio_line++;
    }

    while(pwm->tim != 0)
    {
        if (GPIO_pwm_cfg(pwm) < 0)
        {
            return -1;
        }
        pwm++;
    }
    
    //GPIO_pwm_config();
    return 0;
}

int GPIO_set_mode(TERM_gpio_port_info_TYP* gpio_line)
{
    int rc = 0;
    int gpio;
    GPIO_TypeDef *port;
    TIM_TypeDef *tim;
    int chan;
    TIM_HandleTypeDef *timHandle;
    
    
    rc = GPIO_gpio_convert_to_hal(gpio_line, &port, &gpio);
    if(rc < 0)
        return rc;
    if(!gpio_line->is_PWM)
    {
        GPIO_set_gpio_mode(port, gpio, gpio_line->level);
    }
    else
    {
        rc = GPIO_tim_convert_to_hal(gpio_line->pwm_info,
                                     &gpio_line->idx,
                                     &tim,
                                     &chan);
        if(rc < 0)
            return rc;

        timHandle = &TIM_PWM_handler[(int)gpio_line->pwm_info->tim-1];
        GPIO_set_gpio_pwm_mode(port, gpio);
        GPIO_set_tim_pwm_mode(timHandle, chan, gpio_line->duty);
        
    }

    return rc;
}


extern int GPIO_pwm_cfg(TERM_gpio_tim_pwm_info_TYP* pwm_line)
{

    TIM_TypeDef *tim;
    int chan;
    TIM_HandleTypeDef *timHandle = &TIM_PWM_handler[(int)pwm_line->tim-1];
    uint32_t uhPrescalerValue = 0;
    unsigned int period;
    int rc;

    
    rc = GPIO_tim_convert_to_hal(pwm_line,
                                 &pwm_line->channels[0],
                                 &tim,
                                 &chan);
    if(rc < 0)
            return rc;

    period = (TIM_COUNTER_CLOCK/pwm_line->freq)-1;
    
    timHandle->Instance = tim;
    timHandle->Init.Prescaler         = uhPrescalerValue;
    timHandle->Init.Period            = period;//1999;//PERIOD_VALUE;
    timHandle->Init.ClockDivision     = 0;
    timHandle->Init.CounterMode       = TIM_COUNTERMODE_UP;
    timHandle->Init.RepetitionCounter = 0;
    timHandle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(timHandle) != HAL_OK)
    {
        return -1;
    }
    return 0;
}
