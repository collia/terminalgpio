/**
 * @file    gpio_tim.c
 * @author  Nikolay
 * @version 0.0.1
 * @date    2019-08-04
 * @brief   File contais calculations needed for PWM
 *
 * This function in separate file for simplification 
 * testing
 */

#include <stdint.h>
#include <stdbool.h>
#include "gpio_tim.h"
//#include <stdio.h>
#define TIM_COUNTER_CLOCK 10000

static unsigned int rounded_div(unsigned int a, unsigned int b)
{
    unsigned int rem = a%b;
    unsigned int quot = a/b;
    if(rem > (b >> 1))
    {
        quot += 1;
    }
    return quot;
}

static int calculate_freq(uint32_t system_clock, uint32_t prescaler, uint32_t period, uint32_t clock_division)
{
    uint32_t tim_tick_hz =  rounded_div(system_clock, (prescaler+1));
    if(tim_tick_hz == 0)
    {
        return 0;
    }
    uint32_t pwm_hz = rounded_div(10*tim_tick_hz, (period +1));
    //printf("tick=%d, hz=%d\n", tim_tick_hz, pwm_hz);
    return (pwm_hz);
}

//#define _OLD_
/**
 * @brief         Funciton calculates paramters for tim
 * @param[in]     system_clock
 * @param[in]     freq - needed frequency for pwm
 * @param[out]    prescaler
 * @param[out]    period
 * @param[out]    clock_division
 *
 * @return        0 for OK
 */
int GPIO_calc_tim_pwm_params(uint32_t system_clock,
                             uint32_t freq,
                             uint32_t *prescaler,
                             uint32_t *period,
                             uint32_t *clock_division)
{
#ifdef _OLD_
    *prescaler = (uint32_t)(system_clock / TIM_COUNTER_CLOCK) - 1;
    *period = (((10*TIM_COUNTER_CLOCK)/freq))-1;
    *clock_division = 0;
    return 0;
#else
    int i;
    uint32_t new_prescaler;
    uint32_t new_period;
    bool is_found = false;
    int increment = freq;
    if(freq > 5000)
        increment /= 10;

    for(i = 1; i < system_clock; i+=increment)
    {
        new_prescaler = (uint32_t)(rounded_div(system_clock , i)) - 1;
        if((new_prescaler == 0) || (new_prescaler >= 0xffff))
        {
            continue;
        }

        new_period = (rounded_div((10*i), freq)) -1;
        if((new_period == 0) || (new_period >= 0xffff))
        {
            continue;
        }

        if(new_period < 100)
        {
            continue;
        }

        if(freq != calculate_freq(system_clock, new_prescaler, new_period, 0))
        {
            continue;
        }
        is_found = true;
        *prescaler = new_prescaler;
        *period = new_period;
        *clock_division = 0;
        break;
    }

    if(is_found)
        return 0;
    else
        return -1;
#endif

}
