
#include "stm32f1xx_hal.h"
#include "termgpio.h"
#include "gpio.h"


static int GPIO_get_convert_to_hal(TERM_gpio_port_info_TYP *info,
                                   GPIO_TypeDef **port,
                                   int* gpio)
{
    switch(info->line)
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

    switch(info->port)
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


int GPIO_init(TERM_gpio_port_info_TYP* gpio_table)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    TERM_gpio_port_info_TYP *gpio_line = gpio_table;
    int gpio;
    GPIO_TypeDef *port;
    int rc = 0;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
        
    while((gpio_line->port != 0) &&
          (gpio_line->line != 0))
    {
        rc = GPIO_get_convert_to_hal(gpio_line, &port, &gpio);
        if(rc < 0)
            return rc;

           GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
           GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
           GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
           GPIO_InitStruct.Pin = gpio;
           HAL_GPIO_Init(port, &GPIO_InitStruct);

           GPIO_set_mode(gpio_line);
        gpio_line++;
    }
    return rc;
}

int GPIO_set_mode(TERM_gpio_port_info_TYP* gpio_line)
{
    int rc = 0;
    int gpio;
    GPIO_TypeDef *port;

    rc = GPIO_get_convert_to_hal(gpio_line, &port, &gpio);
    if(rc < 0)
        return rc;

    if(gpio_line->level)
        HAL_GPIO_WritePin(port, gpio, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(port, gpio, GPIO_PIN_RESET);

    return rc;
}
