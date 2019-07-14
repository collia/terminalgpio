#ifndef _TERMGPIO_H_
#define _TERMGPIO_H_

//#include <stdbool.h>
#include <stdlib.h>
typedef unsigned char bool;
#define true 1
#define false 0

//typedef unsigned int size_t;
#define snprintf(...) 

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

int TERM_gpio_atoi(const char *str);
int TERM_gpio_strcmp(const char *str1, const char *str2);
void TERM_gpio_memset(void* data, char val, int len);

void* TERM_gpio_malloc(size_t size);
void* TERM_gpio_realloc(void *ptr, size_t size);
void* TERM_gpio_free(void *ptr);


#endif //_TERMGPIO_H_
