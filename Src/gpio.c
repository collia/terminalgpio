
#include "stm32f1xx_hal.h"
#include "termgpio.h"
#include "gpio.h"
#include "main.h"


#define TIM_COUNTER_CLOCK 10000

TIM_HandleTypeDef TIM_PWM_handler[4]; // TIM1 TIM2 ....


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
        
    while(!((gpio_line->idx.port == 0) &&
            (gpio_line->idx.line == 0)))
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
    uhPrescalerValue = (uint32_t)(SystemCoreClock / TIM_COUNTER_CLOCK) - 1;
    
    period = (TIM_COUNTER_CLOCK*10/pwm_line->freq)-1;
    
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
