#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "gpio_tim.h"
#include "utest.h"

#define SystemCoreClock  72000000U

static double calculate_freq(uint32_t prescaler, uint32_t period, uint32_t clock_division)
{
    double tim_tick_hz = (double)SystemCoreClock / (double)(prescaler+1);
    double freq = (10* tim_tick_hz) / (period + 1);
    return (freq);
}



static bool test_pwm_positive(int waited_freq, double error){
    int rc = 0;
    double back_rc = 0;
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
    if((back_rc < waited_freq-error) ||
       (back_rc > waited_freq+error))
    {
        printf("waited_freq %d != %f +- %f\n", waited_freq, back_rc, error);
        return false;
    }

    return true;
}

static bool test_pwm_important(){
    bool result = true;
    PRINT_RESULT(test_pwm_positive(500, 0.15));
    PRINT_RESULT(test_pwm_positive(600, 0.15));
    PRINT_RESULT(test_pwm_positive(1000, 0.15));
    PRINT_RESULT(test_pwm_positive(1200, 0.15));
    return result;
}

static bool test_pwm_10_0_to_100_0Hz(){
    bool result = true;
    int i;
    for(i = 100; i < 1000; i++)
    {
        result = test_pwm_positive(i, 0.3);
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
        result = test_pwm_positive(i, 0.3);
        if(!result)
            break;
    }
    PRINT_RESULT(result);
    return result;
}

static bool test_pwm_100_0_to_1000_0Hz(){
    bool result = true;
    int i;
    for(i = 1000; i < 10000; i++)
    {
        result = test_pwm_positive(i, 1);
        if(!result)
            break;
    }
    PRINT_RESULT(result);
    return result;
}

static bool test_pwm_1000_0_to_19345_0Hz(){
    bool result = true;
    int i;
    for(i = 10000; i < 19346; i++)
    {
        result = test_pwm_positive(i, 1);
        if(!result)
            break;
    }
    PRINT_RESULT(result);
    return result;
}

static bool test_pwm_hi_freq(){
    bool result = true;
    PRINT_RESULT(test_pwm_positive(2000, 1));

    PRINT_RESULT(test_pwm_positive(2500, 1));
    PRINT_RESULT(test_pwm_positive(3000, 1));
    PRINT_RESULT(test_pwm_positive(3500, 1));
    PRINT_RESULT(test_pwm_positive(4000, 1));
    PRINT_RESULT(test_pwm_positive(4500, 1));
    PRINT_RESULT(test_pwm_positive(5000, 1));
    PRINT_RESULT(test_pwm_positive(6000, 1));

    return result;
}

/* Tested API */
bool pwm_params_buffer_utests() {
    bool rc = true;
    rc &= test_pwm_important();
    rc &= test_pwm_10_0_to_100_0Hz();
    rc &= test_pwm_0_1_to_10_0Hz();
    rc &= test_pwm_100_0_to_1000_0Hz();
    rc &= test_pwm_1000_0_to_19345_0Hz();
    rc &= test_pwm_hi_freq();

    return rc;
}
