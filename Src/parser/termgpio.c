
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "termgpio.h"


#define TERM_PRINT_BUFFER_LENGTH 100

static TERM_gpio_port_info_TYP *TERM_gpio_info_table = 0;

void yyerror(const char *str);
void TERM_debug_print(const char *line);
extern int GPIO_set_mode(TERM_gpio_port_info_TYP* gpio_line);


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
    if(line < 0 || line >= TERM_GPIO_MAX_LINES_NUMBER)
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

            if(GPIO_set_mode(gpio_info) < 0)
            {
                yyerror("Low level error");
                return NULL;
            }
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
    char *bp = buffer;
    if(data->is_PWM)
    {
        //snprintf(buffer, TERM_PRINT_BUFFER_LENGTH,"%c.%d\t%d Hz %d%%\n", data->port, data->line, data->freq, data->duty);
        *bp++=data->port;
        *bp++='.';
        bp += TERM_gpio_itona(data->line, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);  
        *bp++='\t';
        bp += TERM_gpio_itona(data->freq, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
        *bp++=' ';
        *bp++='H';
        *bp++='z';
        *bp++=' ';
        bp += TERM_gpio_itona(data->duty, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
        *bp++='%';
        *bp++='\r';
        *bp++='\n';
        *bp++=0;
        TERM_debug_print(buffer);
    }
    else
    {
        //snprintf(buffer, TERM_PRINT_BUFFER_LENGTH,"%c.%d\t%s\n", data->port, data->line, data->level?"on":"off");
        *bp++=data->port;
        *bp++='.';
        bp += TERM_gpio_itona(data->line, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);  
        *bp++='\t';
        if(data->level)
        {
            *bp++='o';
            *bp++='n';
        }
        else
        {
            *bp++='o';
            *bp++='f';
            *bp++='f';
        }
        *bp++='\r';
        *bp++='\n';
        *bp++=0;
        TERM_debug_print(buffer);
    }
    return 0;
}

// Allowed only simple dec format. returns 0 in error case
int TERM_gpio_atoi(const char *str)
{
    const char *letter = str;;
    int result = 0;
    int sign = 1;
    if(*letter == '-')
    {
        sign = -1;
        ++letter;
    }
    else if (*letter == '+')
    {
        sign = +1;
        ++letter;
    }

    while(*letter != 0)
    {
        if((*letter >= '0') && (*letter <= '9') &&
           (INT_MAX - result > 19))
        {
            result = result *  10 + (*letter - '0');
        }
        else if (*letter == '%')
        {
            return result*sign;
        }
        else
        {
            return 0;
        }
        ++letter;
    }
    
    return result*sign;
}

/**
 * @brif Convert int to string
 * @param[in] value value that will be converted to string
 * @param[in/out] str - buffer where will be stored result
 * @param[in] max_size - size of buffer, when this value 
 *                       be reached will be exit from function
 * @return - len of result string
 */

int TERM_gpio_itona(const int value, char *str, const int max_size )
{
    char *s = str;
    int len = 0;
    int absvalue = value;
    int tmp=value;
    int res;

    if(value == 0)
    {
        *s = '0';
        return 1;
    }
    if(value < 0)
    {
        if(max_size > 0)
        {
            *s = '-';
            ++s;
        }
        absvalue = -value;
    }

    while(tmp!=0)
    {
        tmp = tmp / 10;
        if((len + (s-str)) < max_size)
            len++;
    }
    res = &s[len] - str;
    tmp = absvalue;
    while(tmp!=0)
    {
        if((&s[len-1] - str) < max_size)
            s[len-1] = '0' + tmp % 10;
        tmp /= 10;
        len--;
    }

    return res;
}


int TERM_gpio_strcmp(const char *str1, const char *str2)
{
    const char *s1 = str1;
    const char *s2 = str2;
    while((*s1 == *s2) &&
          (*s1 != 0) &&
          (*s2 != 0))
    {
        s1++;
        s2++;
    }
    if((*s1 == 0) && (*s2 == 0))
    {
        return 0;    
    }
    else
    {
        return s1 - str1 + 1;
    }

//return strcmp(str1, str2);
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
    //printf("size = %zu\n", size);
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
    TERM_gpio_free(ptr);
    return TERM_gpio_malloc(size);
}
void TERM_gpio_free(void *ptr)
{
     if (ptr == memory_pool_8)
     {
         memory_pool_8_used = false;
     }
     if (ptr == memory_pool_64)
     {
         memory_pool_64_used = false;
      }
     if (ptr == memory_pool_1028)
     {
         memory_pool_1028_used = false;
      }
}







