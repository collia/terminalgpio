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


static bool test_pwm_50_0_Hz(){
    bool result = true;
    int rc = 0;
    int back_rc = 0;
    uint32_t prescaler = 0;
    uint32_t period = 0;
    uint32_t clock_division = 0;
    
    rc = GPIO_calc_tim_pwm_params(SystemCoreClock,
                                  500, /* 50.0 */
                                  &prescaler,
                                  &period,
                                  &clock_division);
    
    PRINT_RESULT(rc == 0);
    back_rc = calculate_freq(prescaler, period, clock_division);
    PRINT_RESULT(back_rc == 500);
    
    return result;
}


/* Tested API */
bool pwm_params_buffer_utests() {
    bool rc = true;
    rc &= test_pwm_50_0_Hz();
    return rc;
}
