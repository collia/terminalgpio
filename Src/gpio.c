/**
 * @file    gpio.c
 * @author  Nikolay
 * @version 0.0.1
 * @date    2019-07-28
 * @brief   File contains middle-level api for configuring gpio
 *
 * Function in this file can configure gpio port in gpio output mode
 * with low or hight level or start pwm on selected gpio with corresponding
 * timer
 */

#include "stm32f1xx_hal.h"
#include "termgpio.h"
#include "gpio.h"
#include "main.h"


#define TIM_COUNTER_CLOCK 10000

TIM_HandleTypeDef TIM_PWM_handler[4]; // TIM1 TIM2 ....

/**
 * @brief      Function configure gpio mode
 *
 * @details    In function gpio port is switching to 
 *             gpio output mode and set low or hi lewel
 *
 * @param[in]  port  GPIO interface (GPIOA...etc)
 * @param[in]  gpio  gpio line (GPIO_PIN_1 ... etc)
 * @param[in]  level hi or low gpio state
 *
 * @return     nothing
 */
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
/**
 * @brief      configure port to pwm mode
 *
 * @details    function swith gpio to alternative mode
 *             in alternative mode is started sending TIM
 *
 * @param[in] port Pointer to hal port
 * @param[in] gpio hal gpio line (GPIO_PIN_1 ... etc)
 *
 * @return     nothing
 */
static void GPIO_set_gpio_pwm_mode(GPIO_TypeDef *port, int gpio)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = gpio;
    HAL_GPIO_Init(port, &GPIO_InitStruct);

}
/**
 * @brief         Configure timer chanel in pwm mode
 * @param[in]     handle pointer on tim structure
 *                with timer config
 * @param[in]     chan timer channel to configure
 * @param[in]     duty duty level for channel. Supported
 *                persents from 0.1 to 100.0
 *
 * @return        none
 */
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

/**
 * @brief         Function converts local port presentation to HAL
 * @param[in]     info pointer to gpio structure
 * @param[out]    port HAL port presentation
 * @param[out]    gpio HAL gpio line presentation
 *
 * @return        0 for success, nonzero otherwise
 */
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
/**
 * @brief         Convert internal timer presentation to HAL
 * @param[in]     info - internal timer structure
 * @param[out]    tim - HAL timer
 *
 * @return        0 for ok
 */
static int GPIO_tim_convert_to_hal(TERM_gpio_tim_pwm_info_TYP *info,
                                   TIM_TypeDef **tim)
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
    return 0;
}
/**
 * @brief         Convert internal gpio to tim channel
 * @details
 * @param[in]     info timer
 * @param[in]     idx gpio index
 * @param[out]    channel HAL TIM channel number
 *
 * @return        0 for ok
 */
static int GPIO_tim_channel_convert_to_hal(TERM_gpio_tim_pwm_info_TYP *info,
                                           TERM_gpio_port_TYP *idx,
                                           int* channel)
{
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
/**
 * @brief         General gpio module initialisation
 * @details       Function config all ports and timers
 *                according to initial config
 * @param[in]     gpio_table gpio config
 * @param[in]     pwm_table timers config
 *
 * @return        0 for ok
 */
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
    return 0;
}
/**
 * @brief         Reconfigure gpio line
 * @param[in]     gpio_line structure with needed gpio config
 *
 * @return        0 for OK
 */
int GPIO_set_mode(TERM_gpio_port_info_TYP* gpio_line)
{
    int rc = 0;
    int gpio;
    GPIO_TypeDef *port;
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
        rc = GPIO_tim_channel_convert_to_hal(gpio_line->pwm_info,
                                             &gpio_line->idx,
                                             &chan);
        if(rc < 0)
            return rc;

        timHandle = &TIM_PWM_handler[(int)gpio_line->pwm_info->tim-1];
        GPIO_set_gpio_pwm_mode(port, gpio);
        GPIO_set_tim_pwm_mode(timHandle, chan, gpio_line->duty);
    }

    return rc;
}

/**
 * @brief         Reconfigure PWM timer
 * @details       Function support only changing frequency
 *                Also when this function is called is reconfigured all
 *                gpio lines, connected to this tim
 * @param[in]     pwm_line timer configuration
 *
 * @return        0 for OK
 */
extern int GPIO_pwm_cfg(TERM_gpio_tim_pwm_info_TYP* pwm_line)
{

    TIM_TypeDef *tim;
    TIM_HandleTypeDef *timHandle = &TIM_PWM_handler[(int)pwm_line->tim-1];
    uint32_t uhPrescalerValue = 0;
    unsigned int period;
    int rc;

    rc = GPIO_tim_convert_to_hal(pwm_line,
                                 &tim);
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
