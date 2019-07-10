
#include <stdio.h>
#include "termgpio.h"

static TERM_gpio_port_info_TYP *TERM_gpio_info_table = 0;

void yyerror(const char *str);

TERM_gpio_port_info_TYP*  TERM_gpio_get_info()
{
    return TERM_gpio_info_table;
}

void TERM_gpio_set_info(TERM_gpio_port_info_TYP* gpio_table)
{
    TERM_gpio_info_table = gpio_table;
}



int TERM_gpio_set_mode(int port, int line, bool mode, bool is_PWM, int freq, int duty)
{
    char cport;
    //int num_records = sizeof(TERM_gpio_info_table)/sizeof(TERM_gpio_info_table[0]);
    TERM_gpio_port_info_TYP *gpio_info = TERM_gpio_info_table;

    if(TERM_gpio_info_table == 0)
    {
            yyerror("not initialized");
        return -1;
    }

    if(port >= 0 && port < 4)
    {
        cport = 'A' + port;
    }
    else
    {
        yyerror("incorrect port");
        return -1;
    }
    if(line < 0 || line > TERM_GPIO_MAX_LINES_NUMBER)
    {
        yyerror("incorrect line");
        return -2;
    }

    while((gpio_info->port != 0) &&
      (gpio_info->line != 0))
    {
        if((gpio_info->port == cport) &&
           (gpio_info->line == line))
        {
            gpio_info->is_PWM = is_PWM;
            gpio_info->level = mode;
            gpio_info->freq = freq;
            gpio_info->duty = duty;
            return 0;
        }
    gpio_info++;
    }
    yyerror("Port is not allowed");
    return -3;
}
