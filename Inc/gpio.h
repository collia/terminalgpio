/**
 * @file    gpio.h
 * @author  Nikolay
 * @version 0.0.1
 * @date    2019-07-28
 * @brief   File contains public interface for gpio module
 *
 */

#ifndef _GPIO_H_
#define _GPIO_H_
/**
 * @brief         General gpio module initialisation
 * @details       Function config all ports and timers
 *                according to initial config
 * @param[in]     gpio_table gpio config
 * @param[in]     pwm_table timers config
 *
 * @return        0 for ok
 */
extern int GPIO_init(TERM_gpio_port_info_TYP* gpio_table,
                     TERM_gpio_tim_pwm_info_TYP *pwm_table);
/**
 * @brief         Reconfigure gpio line
 * @param[in]     gpio_line structure with needed gpio config
 *
 * @return        0 for OK
 */
extern int GPIO_set_mode(TERM_gpio_port_info_TYP* gpio_line);

/**
 * @brief         Reconfigure PWM timer
 * @details       Function support only changing frequency
 *                Also when this function is called is reconfigured all
 *                gpio lines, connected to this tim
 * @param[in]     pwm_line timer configuration
 *
 * @return        0 for OK
 */
extern int GPIO_pwm_cfg(TERM_gpio_tim_pwm_info_TYP* pwm_line);
#endif
