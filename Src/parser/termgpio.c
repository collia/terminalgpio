
#include <stdio.h>
#include "termgpio.h"

static TERM_gpio_port_info_TYP gpio_info[10] =
{
    {'A', 1, false, false, 0, 0},
    {'B', 2, false, false, 0, 0},
    {'B', 3, false, true, 0, 0},
    {'B', 4, true,  false, 50, 50},
    {0, 0, false, false, 0, 0},
};

TERM_gpio_port_info_TYP*  TERM_gpio_info()
{
    return gpio_info;
}


int TERM_gpio_set_mode(int port, int line, bool mode, bool is_PWM, int freq, int duty)
{
    char cport;
    int num_records = sizeof(gpio_info)/sizeof(gpio_info[0]);
    if(port >= 0 && port < 4)
    {
        cport = 'A' + port;
    }
    else
    {
        printf("Error: incorrect port %d\n", port);
        return -1;
    }
    if(line < 0 || line > TERM_GPIO_MAX_LINES_NUMBER)
    {
        printf("Error: incorrect line %d\n", line);
        return -2;
    }

    for(int i = 0; i < num_records; i++)
    {
        if((gpio_info[i].port == cport) &&
           (gpio_info[i].line == line))
        {
            gpio_info[i].is_PWM = is_PWM;
            gpio_info[i].level = mode;
            gpio_info[i].freq = freq;
            gpio_info[i].duty = duty;
            return 0;
        }
    }
    printf("Error: cannot find \n");
    return -3;
}
