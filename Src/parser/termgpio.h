#ifndef _TERMGPIO_H_
#define _TERMGPIO_H_

#include <stdbool.h>

#define TERM_GPIO_MAX_LINES_NUMBER 16

typedef struct
{
    char port;
    int line;
    bool is_PWM;
    bool level;
    int freq;
    int duty;
} TERM_gpio_port_info_TYP;

TERM_gpio_port_info_TYP* TERM_gpio_get_info();
void TERM_gpio_set_info(TERM_gpio_port_info_TYP* gpio_table);
int TERM_gpio_set_mode(int port, int line, bool mode, bool is_PWM, int freq, int duty);

int TERM_parser();

#endif //_TERMGPIO_H_
