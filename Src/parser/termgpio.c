/**
 * @file    termgpio.c
 * @author  Nikolay
 * @version 0.0.1
 * @date    2019-07-28
 * @brief   File contains functions for terminal and implements
 *           needed posix functions
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "termgpio.h"


#define TERM_PRINT_BUFFER_LENGTH 200

static TERM_gpio_port_info_TYP *TERM_gpio_info_table = 0;
static TERM_gpio_tim_pwm_info_TYP *TERM_gpio_pwm_table = 0;

extern void yyerror(const char *str);
extern void TERM_debug_print(const char *line);

extern int GPIO_set_mode(TERM_gpio_port_info_TYP* gpio_line);
extern int GPIO_pwm_cfg(TERM_gpio_tim_pwm_info_TYP* pwm_line);

/**
 * @brief         Scan tim table and return applicable timer or null
 * @details       module should be initialized before this function call
 * @param[in]     idx gpio entry
 *
 * @return        tim structure pointer or null
 */
static TERM_gpio_tim_pwm_info_TYP* TERM_gpio_find_tim_for_port(
    const TERM_gpio_port_TYP* idx)
{
    TERM_gpio_tim_pwm_info_TYP * tim = TERM_gpio_pwm_table;
    while(tim->tim != 0)
    {
        for(int i = 0; i < TERM_GPIO_MAX_PWM_CH_NUMBER; i++)
        {
            if((tim->channels[i].port == idx->port) &&
               (tim->channels[i].line == idx->line))
            {
                return tim;
            }
        }
        tim++;
    }
    return NULL;
}
/**
 * @brief         Find in gpio table element for port letter and line
 * @details       can return null, should be called after table initialization
 * @param[in]     idx gpio port name and line number
 *
 * @return        gpio entry ptr or null
 */
static TERM_gpio_port_info_TYP* TERM_gpio_find_gpio_for_port(
    const TERM_gpio_port_TYP* idx)
{
    TERM_gpio_port_info_TYP* gpio_info = TERM_gpio_info_table;
    while(!((gpio_info->idx.port == 0) &&
            (gpio_info->idx.line == 0)))
    {
        if((gpio_info->idx.port == idx->port) &&
           (gpio_info->idx.line == idx->line))
        {
            return gpio_info;
        }
        gpio_info++;
    }
    return NULL;
}
/**
 * @brief         Return ptr for gpio table
 * @details       If module didn't initialized, can return null
 *
 * @return        gpio table
 */
TERM_gpio_port_info_TYP*  TERM_gpio_get_gpio_info()
{
    return TERM_gpio_info_table;
}
/**
 * @brief         Return ptr for timers table
 * @details       If module didn't initialized, can return null
 *
 * @return        timers table
 */
TERM_gpio_tim_pwm_info_TYP*  TERM_gpio_get_pwm_info()
{
    return TERM_gpio_pwm_table;
}
/**
 * @brief         Initialisation of module
 *
 * @param[in]     gpio_table pointer to gpio table in static memory
 * @param[in]     pwm_table pointer to timer table in static memory
 *
 * @return        none
 */
void TERM_gpio_set_info(TERM_gpio_port_info_TYP* gpio_table,
                        TERM_gpio_tim_pwm_info_TYP *pwm_table)
{
    TERM_gpio_info_table = gpio_table;
    TERM_gpio_pwm_table = pwm_table;
}
/**
 * @brief         Handler for gpio port mode function
 * @details       Function is called when user input commands:
 *                gpio A|B|C|D port [0-15] mode on|off
 *                gpio A|B|C|D port [0-15] mode pwm [0-100]%
 *
 * @param[in]     port "A|B|C|D" char port presentation
 * @param[in]     line 0-15 gpio line
 * @param[in]     mode in gpio mode hi or low level
 * @param[in]     is_PWM gpio line mode - gpio output or PWM
 * @param[in]     duty in pwm mode duty for gpio line
 *
 * @return        pointer to updated table line, or NULL when error
 */
TERM_gpio_port_info_TYP * TERM_gpio_set_mode(int port, int line, bool mode, bool is_PWM, int duty)
{
    char cport;

    TERM_gpio_port_info_TYP *gpio_info = TERM_gpio_info_table;
    TERM_gpio_tim_pwm_info_TYP *tim_info = NULL;
    if((TERM_gpio_info_table == 0) || (TERM_gpio_pwm_table == 0))
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

    if(duty < 0 || duty > 100)
    {
        yyerror("incorrect persent");
        return NULL;
    }

    while(!((gpio_info->idx.port == 0) &&
            (gpio_info->idx.line == 0)))
    {
        if((gpio_info->idx.port == cport) &&
           (gpio_info->idx.line == line))
        {
            if(!is_PWM)
            {
                gpio_info->level = mode;
                gpio_info->is_PWM = false;
            }
            else
            {
                tim_info = TERM_gpio_find_tim_for_port(&gpio_info->idx);
                if(tim_info != 0)
                {
                    gpio_info->is_PWM = is_PWM;
                    gpio_info->duty = duty;
                    gpio_info->pwm_info = tim_info;
                }
                else
                {
                    yyerror("PWM is not allowed on this port");
                    return NULL;
                }
            }
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
/**
 * @brief         Handler for pwm freq command
 * @details       Function is called when user input commands:
 *                pwm tim [1-3] freq <int>
 *
 * @param[in]     tim_num tim number 0-4
 * @param[in]     freq new frequency for timer TIM
 *
 * @return        pointer to updated table line, or NULL when error
 */
TERM_gpio_tim_pwm_info_TYP * TERM_gpio_set_pwm_freq(int tim_num, int freq)
{
    TERM_gpio_tim_pwm_info_TYP * tim = TERM_gpio_pwm_table;
    TERM_gpio_port_info_TYP *gpio_info = TERM_gpio_info_table;

    if((TERM_gpio_info_table == 0) || (TERM_gpio_pwm_table == 0))
    {
        yyerror("not initialized");
        return NULL;
    }

    if(freq < 0)
    {
        yyerror("incorrect freq");
        return NULL;
    }
    if(tim_num < 0 || tim_num > 4)
    {
        yyerror("incorrect tim");
        return NULL;
    }
    while(tim->tim != 0)
    {
        for(int i = 0; i < TERM_GPIO_MAX_PWM_CH_NUMBER; i++)
        {
            if((tim->tim == tim_num))
            {
                tim->freq = freq;

                if(GPIO_pwm_cfg(tim) <0)
                {
                    yyerror("TIM low level error");
                    return NULL;
                }

                while(!((gpio_info->idx.port == 0) &&
                        (gpio_info->idx.line == 0)))
                {
                    if((gpio_info->is_PWM) && (gpio_info->pwm_info != NULL))
                    {
                        if(gpio_info->pwm_info->tim == tim->tim)
                        {
                            if(GPIO_set_mode(gpio_info) < 0)
                            {
                                yyerror("GPIO low level error");
                                return NULL;
                            }
                        }
                    }
                    gpio_info++;
                }
                return tim;
            }
        }
        tim++;
    }
    yyerror("tim is not allowed");
    return NULL;
}
/**
 * @brief         Print to terminal information about gpio line
 * @details       This function is called for command
 *                gpio info
 *                and config functions
 * @param[in]     data gpio info that will be printed
 *
 * @return        0 for success
 */
int TERM_gpio_print_port_info(TERM_gpio_port_info_TYP * data)
{
    char buffer[TERM_PRINT_BUFFER_LENGTH];
    char *bp = buffer;
    if(data->is_PWM)
    {

        *bp++=data->idx.port;
        *bp++='.';
        bp += TERM_gpio_itona(data->idx.line, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
        *bp++='\t';
        *bp++='p';
        *bp++='w';
        *bp++='m';
        *bp++=' ';
        bp += TERM_gpio_itona(data->duty, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
        *bp++='%';
        *bp++=' ';
        *bp++='t';
        *bp++='i';
        *bp++='m';
        bp += TERM_gpio_itona(data->pwm_info->tim, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
        *bp++=' ';
        bp += TERM_gpio_itona(data->pwm_info->freq/10, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
        *bp++='.';
        bp += TERM_gpio_itona(data->pwm_info->freq%10, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
        *bp++='H';
        *bp++='z';
        *bp++='\r';
        *bp++='\n';
        *bp++=0;
        TERM_debug_print(buffer);
    }
    else
    {
        *bp++=data->idx.port;
        *bp++='.';
        bp += TERM_gpio_itona(data->idx.line, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
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
/**
 * @brief         Print to terminal information about timer
 * @details       This function is called for command
 *                gpio info
 *                and config functions
 * @param[in]     data gpio info that will be printed
 *
 * @return        0 for success
 */
int TERM_gpio_print_tim_info(TERM_gpio_tim_pwm_info_TYP * data)
{
    TERM_gpio_port_info_TYP *gpio_info;
    char buffer[TERM_PRINT_BUFFER_LENGTH];
    char *bp = buffer;
    *bp++='T';
    *bp++='I';
    *bp++='M';
    bp += TERM_gpio_itona(data->tim, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
    *bp++=' ';
    *bp++='F';
    *bp++='r';
    *bp++='e';
    *bp++='q';
    *bp++=' ';
    bp += TERM_gpio_itona(data->freq/10, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
    *bp++='.';
    bp += TERM_gpio_itona(data->freq%10, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
    *bp++='H';
    *bp++='z';
    *bp++=':';
    *bp++='\r';
    *bp++='\n';
    for(int i = 0; i < TERM_GPIO_MAX_PWM_CH_NUMBER; i++)
    {
        gpio_info = TERM_gpio_find_gpio_for_port(&data->channels[i]);
        if(gpio_info)
        {
            *bp++='\t';
            *bp++=gpio_info->idx.port;
            *bp++='.';
            bp += TERM_gpio_itona(gpio_info->idx.line, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
            *bp++='\t';
            *bp++='p';
            *bp++='w';
            *bp++='m';
            *bp++=' ';
            if(gpio_info->is_PWM)
            {
                bp += TERM_gpio_itona(gpio_info->duty, bp, &buffer[TERM_PRINT_BUFFER_LENGTH]-bp-2);
                *bp++='%';
            }
            else
            {
                *bp++='o';
                *bp++='f';
                *bp++='f';
            }
            *bp++='\r';
            *bp++='\n';

        }
    }
    *bp++=0;
    TERM_debug_print(buffer);
    return 0;
}


/** @defgroup termposix Posix functions for terminal
 *  Generated  lexer and parser needs some posix functions
 *  but embedded platforms doesn't contains them
 *  @{
 */
/**
 * @brief         function converts string to integer
 * @details       Allowed only simple dec format. returns 0 in error case
 *                char '%' stops converting, so 100% will be 100
 *                char '.' is skipped, so 10.1 will be 101
 * @param[in]     string with number
 *
 * @return        converted number or 0
 */
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
        else if(*letter == '.')
        {
            // Do nothing - this case for fixed point numbers
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
/**
 * @brief         Compare two strings
 * @details       simylation of standart strcmp function
 * @param[in]     str1 first C string to comare
 * @param[in]     str2 second C string to comare
 *
 * @return        0 in case strings are equal,
 *               nonzero - index of first nonequal char plus one
 */
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
}
/**
 * @brief         Set memory with some value
 * @details       wrapper for memset function
 * @param[in]     data - ptr to memory
 * @param[in]     val - value to set
 * @param[in]     len - number of bytes to write
 *
 * @return       none
 */
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
/**
 * @brief         memory allocate
 * @details       simulation af malloc function
 *                Can allocate only 3 buffers with sizes 8, 64 and 1028 bytes
 * @param[in]     size - buffer size to allocate
 *
 * @return        pointer to memory or null
 */
void* TERM_gpio_malloc(size_t size)
{
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
/**
 * @brief         memory allocate
 * @details       simulation af realloc function
 *                Can allocate only 3 buffers with sizes 8, 64 and 1028 bytes
 * @param[in]     ptr - pointer to memory
 * @param[in]     size - buffer size to allocate
 *
 * @return        pointer to memory or null
 */
void* TERM_gpio_realloc(void *ptr, size_t size)
{
    TERM_gpio_free(ptr);
    return TERM_gpio_malloc(size);
}
/**
 * @brief         memory free
 * @details       simulation af free function
 * @param[in]     ptr - pointer to memory
 *
 * @return        nothing
 */
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
/** @} */
