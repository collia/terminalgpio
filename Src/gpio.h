#ifndef _GPIO_H_
#define _GPIO_H_

extern int GPIO_init(TERM_gpio_port_info_TYP* gpio_table,
                     TERM_gpio_tim_pwm_info_TYP *pwm_table);
extern int GPIO_set_mode(TERM_gpio_port_info_TYP* gpio_line);
extern int GPIO_pwm_cfg(TERM_gpio_tim_pwm_info_TYP* pwm_line);

    
#endif
