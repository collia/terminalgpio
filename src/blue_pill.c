
/**
File contains controls for evaluation board http://wiki.stm32duino.com/index.php?title=Blue_Pill

*/
#include "termgpio.h"
#include "blue_pill.h"

/*
Test commands
gpio A port 0 mode off
gpio A port 1 mode off
gpio A port 2 mode off
gpio A port 3 mode off
gpio A port 4 mode off
gpio A port 5 mode off
gpio A port 6 mode off
gpio A port 7 mode off
gpio A port 8 mode off
gpio A port 9 mode off
gpio A port 10 mode off
gpio A port 15 mode off

gpio B port 0 mode off
gpio B port 1 mode off
gpio B port 3 mode off
gpio B port 4 mode off
gpio B port 5 mode off
gpio B port 6 mode off
gpio B port 7 mode off
gpio B port 8 mode off
gpio B port 9 mode off
gpio B port 10 mode off
gpio B port 11 mode off
gpio B port 12 mode off
gpio B port 15 mode off

gpio C port 14 mode off
gpio C port 15 mode off


gpio A port 0 mode on
gpio A port 1 mode on
gpio A port 2 mode on
gpio A port 3 mode on
gpio A port 4 mode on
gpio A port 5 mode on
gpio A port 6 mode on
gpio A port 7 mode on
gpio A port 8 mode on
gpio A port 9 mode on
gpio A port 10 mode on
gpio A port 15 mode on

gpio B port 0 mode on
gpio B port 1 mode on
gpio B port 3 mode on
gpio B port 4 mode on
gpio B port 5 mode on
gpio B port 6 mode on
gpio B port 7 mode on
gpio B port 8 mode on
gpio B port 9 mode on
gpio B port 10 mode on
gpio B port 11 mode on
gpio B port 12 mode on
gpio B port 15 mode on

gpio C port 13 mode on
gpio C port 14 mode on
gpio C port 15 mode on

gpio A port 0 mode pwm 100%
gpio A port 0 mode pwm 90%
gpio A port 0 mode pwm 70%
gpio A port 0 mode pwm 50%
gpio A port 0 mode pwm 30%
gpio A port 0 mode pwm 20%
gpio A port 0 mode pwm 0%

gpio A port 1 mode pwm 50%
gpio A port 2 mode pwm 50%
gpio A port 3 mode pwm 50%

pwm tim 2 freq 10

*/

TERM_gpio_port_info_TYP BRD_gpio_def_config[] =
{
    // LED
    //{{'C', 13},  true, false, 0},
    {{'C', 14},  true, false, 0},
    {{'C', 15},  true, false, 0},
    {{'A', 0},   false, false, 0},
    {{'A', 1},   false, false, 0},
    {{'A', 2},   false, false, 0},
    {{'A', 3},   false, false, 0},
    {{'A', 4},   true, false, 0},
    {{'A', 5},   true, false, 0},
    {{'A', 6},   true, false, 0},
    {{'A', 7},   true, false, 0},
    {{'B', 0},   true, false, 0},
    {{'B', 1},   true, false, 0},
    {{'B', 10},   true, false, 0},
    {{'B', 11},   true, false, 0},

    {{'B', 9},   true, false, 0},
    {{'B', 8},   true, false, 0},
    {{'B', 7},   true, false, 0},
    {{'B', 6},   true, false, 0},
    {{'B', 5},   true, false, 0},
    {{'B', 4},   true, false, 0},
    // B.3 doe
    //{{'B', 3},   true, false, 0},
    {{'A', 15},   true, false, 0},
    // USB
    //{{'A', 12},   true, false, 0},
    //{{'A', 11},   true, false, 0},
    {{'A', 10},   true, false, 0},
    {{'A', 9},   true, false, 0},
    {{'A', 8},   true, false, 0},
    {{'B', 15},   true, false, 0},
    {{'B', 14},   true, false, 0},
    {{'B', 13},   true, false, 0},
    {{'B', 12},   true, false, 0},

    {{0, 0}, false, false, 0},

};

TERM_gpio_tim_pwm_info_TYP BRD_gpio_pwm_def_config[] =
{
    {2, 1000, {{'A', 0}, {'A', 1},{'A', 2},{'A', 3}}},
    {0, 0,  {{0, 0}, {0, 0},{0, 0},{0, 0}}}
};

void BRD_led_init()
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    LED1_GPIO_CLK_ENABLE();

    /* -2- Configure IO in output push-pull mode to drive external LEDs */
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = LED1_PIN;
    HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
}




void BRD_led_toggle()
{
    HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
}

void BRD_led_on()
{
    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
}

void BRD_led_off()
{
    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
}
