#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "gpio_tim.h"
#include "utest.h"

#define SystemCoreClock  72000000U

static int calculate_freq(uint32_t prescaler, uint32_t period, uint32_t clock_division)
{
    double tim_tick_ms = (double)(prescaler+1) / (double)SystemCoreClock;
    double pwm_ms = tim_tick_ms * (period + 1);
    double freq = 1 / pwm_ms;
    return (int)(freq*10);
}



static bool test_pwm_positive(int waited_freq){
    int rc = 0;
    int back_rc = 0;
    uint32_t prescaler = 0;
    uint32_t period = 0;
    uint32_t clock_division = 0;
    
    rc = GPIO_calc_tim_pwm_params(SystemCoreClock,
                                  waited_freq,
                                  &prescaler,
                                  &period,
                                  &clock_division);
    
    if(rc != 0)
    {
        printf("Freq=%d, rc = %d\n", waited_freq, rc);
        return false;
    }
    back_rc = calculate_freq(prescaler, period, clock_division);
    if(prescaler == 0 || prescaler > 0xffff)
    {
        printf("incorrect prescaler(%d): 0x%x\n", waited_freq, prescaler);
        return false;
    }
    if(period == 0 || period > 0xffff)
    {
        printf("incorrect period: 0x%x\n", period);
        return false;
    }
    if(clock_division > 0xffff)
    {
        printf("incorrect clock_division: 0x%x\n", clock_division);
        return false;
    }
    if(back_rc != waited_freq)
    {
        printf("waited_freq %d != %d\n", waited_freq, back_rc);
        return false;
    }

    return true;
}

static bool test_pwm_50_0_Hz(){
    bool result = true;
    PRINT_RESULT(test_pwm_positive(500));
    return result;
}

static bool test_pwm_10_0_to_100_0Hz(){
    bool result = true;
    int i;
    for(i = 100; i < 1000; i++)
    {
        result = test_pwm_positive(i);
        if(!result)
            break;
    }
    PRINT_RESULT(result);
    return result;
}

static bool test_pwm_0_1_to_10_0Hz(){
    bool result = true;
    int i;
    for(i = 1; i < 100; i++)
    {
        result = test_pwm_positive(i);
        if(!result)
            break;
    }
    PRINT_RESULT(result);
    return result;
}

/* Tested API */
bool pwm_params_buffer_utests() {
    bool rc = true;
    rc &= test_pwm_50_0_Hz();
    rc &= test_pwm_10_0_to_100_0Hz();
    rc &= test_pwm_0_1_to_10_0Hz();
    return rc;
}
