/**
 * @file    gpio_tim.h
 * @author  Nikolay
 * @version 0.0.1
 * @date    2019-08-04
 * @brief   File contais calculations needed for PWM
 *
 * This function in separate file for simplification 
 * testing
 */

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
extern int GPIO_calc_tim_pwm_params(uint32_t system_clock,
                                    uint32_t freq,
                                    uint32_t *prescaler,
                                    uint32_t *period,
                                    uint32_t *clock_division);

