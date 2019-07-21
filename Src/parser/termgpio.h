#ifndef _TERMGPIO_H_
#define _TERMGPIO_H_

//#include <stdbool.h>
#include <stdlib.h>
typedef unsigned char bool;
#define true 1
#define false 0

//typedef unsigned int size_t;

#define TERM_GPIO_MAX_LINES_NUMBER 16
#define TERM_GPIO_MAX_PWM_CH_NUMBER 4

typedef struct
{
    char port;
    char line;
} TERM_gpio_port_TYP;

typedef struct
{
    char tim;
    unsigned int freq;
    TERM_gpio_port_TYP channels[TERM_GPIO_MAX_PWM_CH_NUMBER];
} TERM_gpio_tim_pwm_info_TYP;

typedef struct
{
    TERM_gpio_port_TYP idx;
    bool level;
    bool is_PWM;
    int duty;
    TERM_gpio_tim_pwm_info_TYP * pwm_info;
} TERM_gpio_port_info_TYP;


TERM_gpio_port_info_TYP* TERM_gpio_get_gpio_info();
TERM_gpio_tim_pwm_info_TYP*  TERM_gpio_get_pwm_info();
void TERM_gpio_set_info(TERM_gpio_port_info_TYP* gpio_table, TERM_gpio_tim_pwm_info_TYP* pwm_table);
TERM_gpio_port_info_TYP* TERM_gpio_set_mode(int port, int line, bool mode, bool is_PWM, int duty);
TERM_gpio_tim_pwm_info_TYP * TERM_gpio_set_pwm_freq(int tim, int freq);
int TERM_gpio_print_port_info(TERM_gpio_port_info_TYP * data);
int TERM_gpio_print_tim_info(TERM_gpio_tim_pwm_info_TYP * data);


int TERM_parser();

int TERM_gpio_atoi(const char *str);
int TERM_gpio_itona(const int value, char *str, const int max_size );
int TERM_gpio_strcmp(const char *str1, const char *str2);
void TERM_gpio_memset(void* data, char val, int len);

void* TERM_gpio_malloc(size_t size);
void* TERM_gpio_realloc(void *ptr, size_t size);
void TERM_gpio_free(void *ptr);


#endif //_TERMGPIO_H_
