
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "termgpio.h"

#define TERM_PRINT_BUFFER_LENGTH 100

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



TERM_gpio_port_info_TYP * TERM_gpio_set_mode(int port, int line, bool mode, bool is_PWM, int freq, int duty)
{
    char cport;

    TERM_gpio_port_info_TYP *gpio_info = TERM_gpio_info_table;

    if(TERM_gpio_info_table == 0)
    {
        yyerror("not initialized");
        return NULL;
    }

    if(port >= 0 && port < 4)
    {
        cport = 'A' + port;
    }
    else
    {
        yyerror("incorrect port");
        return NULL;
    }
    if(line < 0 || line > TERM_GPIO_MAX_LINES_NUMBER)
    {
        yyerror("incorrect line");
        return NULL;
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
            return gpio_info;
        }
    gpio_info++;
    }
    yyerror("Port is not allowed");
    return NULL;
}


int TERM_gpio_print_port_info(TERM_gpio_port_info_TYP * data)
{
    char buffer[TERM_PRINT_BUFFER_LENGTH];
    if(data->is_PWM)
    {
        snprintf(buffer, TERM_PRINT_BUFFER_LENGTH,"%c.%d\t%d Hz %d%%\n", data->port, data->line, data->freq, data->duty);
        TERM_debug_print(buffer);
    }
    else
    {
        snprintf(buffer, TERM_PRINT_BUFFER_LENGTH,"%c.%d\t%s\n", data->port, data->line, data->level?"on":"off");
        TERM_debug_print(buffer);
    }
    return 0;
}


int TERM_gpio_atoi(const char *str)
{
    return atoi(str);
}

int TERM_gpio_strcmp(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}

void TERM_gpio_memset(void* data, char val, int len)
{
     memset(data, val, len);
}

static char memory_pool_8[8];
static bool memory_pool_8_used = false;
static char memory_pool_64[64];
static bool memory_pool_64_used = false;
static char memory_pool_1028[1028];
static bool memory_pool_1028_used = false;

void* TERM_gpio_malloc(size_t size)
{
    printf("size = %zu\n", size);
    if (size <= 8)
    {
        if (memory_pool_8_used)
        {
            return 0;
        }
        else
        {
            memory_pool_8_used = true;
            return memory_pool_8;
        }
    }
    else if (size <= 64)
    {
        if (memory_pool_64_used)
        {
            return 0;
        }
        else
        {
            memory_pool_64_used = true;
            return memory_pool_64;
        }
    }
    else if (size <= 1028)
    {
        if (memory_pool_1028_used)
        {
            return 0;
        }
        else
        {
            memory_pool_1028_used = true;
            return memory_pool_1028;
        }
    }
    return 0;
}

void* TERM_gpio_realloc(void *ptr, size_t size)
{
    return 0;
}
void* TERM_gpio_free(void *ptr)
{
    return 0;
}







