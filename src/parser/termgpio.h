/**
 * @file    termgpio.h
 * @author  Nikolay
 * @version 0.0.1
 * @date    2019-07-28
 * @brief   File contains declaration functions for terminal ands
 *           needed posix functions
 *
 *
 */
#ifndef _TERMGPIO_H_
#define _TERMGPIO_H_


#include <stdlib.h>

typedef unsigned char bool;
#define true 1
#define false 0

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

/**
 * @brief         Return ptr for gpio table
 * @details       If module didn't initialized, can return null
 *
 * @return        gpio table
 */
TERM_gpio_port_info_TYP* TERM_gpio_get_gpio_info();
/**
 * @brief         Return ptr for timers table
 * @details       If module didn't initialized, can return null
 *
 * @return        timers table
 */
TERM_gpio_tim_pwm_info_TYP*  TERM_gpio_get_pwm_info();
/**
 * @brief         Initialisation of module
 *
 * @param[in]     gpio_table pointer to gpio table in static memory
 * @param[in]     pwm_table pointer to timer table in static memory
 *
 * @return        none
 */
void TERM_gpio_set_info(TERM_gpio_port_info_TYP* gpio_table, TERM_gpio_tim_pwm_info_TYP* pwm_table);
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
TERM_gpio_port_info_TYP* TERM_gpio_set_mode(int port, int line, bool mode, bool is_PWM, int duty);
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
TERM_gpio_tim_pwm_info_TYP * TERM_gpio_set_pwm_freq(int tim, int freq);
/**
 * @brief         Print to terminal information about gpio line
 * @details       This function is called for command
 *                gpio info
 *                and config functions
 * @param[in]     data gpio info that will be printed
 *
 * @return        0 for success
 */
int TERM_gpio_print_port_info(TERM_gpio_port_info_TYP * data);
/**
 * @brief         Print to terminal information about timer
 * @details       This function is called for command
 *                gpio info
 *                and config functions
 * @param[in]     data gpio info that will be printed
 *
 * @return        0 for success
 */
int TERM_gpio_print_tim_info(TERM_gpio_tim_pwm_info_TYP * data);
/**
 * @brief         Main function for parser
 * @details       implementation is in autogenerated lexer
 *
 * @return        none
 */
int TERM_parser();
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
int TERM_gpio_atoi(const char *str);
/**
 * @brif Convert int to string
 * @param[in] value value that will be converted to string
 * @param[in/out] str - buffer where will be stored result
 * @param[in] max_size - size of buffer, when this value 
 *                       be reached will be exit from function
 * @return - len of result string
 */
int TERM_gpio_itona(const int value, char *str, const int max_size );
/**
 * @brief         Compare two strings
 * @details       simylation of standart strcmp function
 * @param[in]     str1 first C string to comare
 * @param[in]     str2 second C string to comare
 *
 * @return        0 in case strings are equal,
 *               nonzero - index of first nonequal char plus one
 */
int TERM_gpio_strcmp(const char *str1, const char *str2);
/**
 * @brief         Set memory with some value
 * @details       wrapper for memset function
 * @param[in]     data - ptr to memory
 * @param[in]     val - value to set
 * @param[in]     len - number of bytes to write
 *
 * @return       none
 */
void TERM_gpio_memset(void* data, char val, int len);
/**
 * @brief         memory allocate
 * @details       simulation af malloc function
 *                Can allocate only 3 buffers with sizes 8, 64 and 1028 bytes
 * @param[in]     size - buffer size to allocate
 *
 * @return        pointer to memory or null
 */
void* TERM_gpio_malloc(size_t size);
/**
 * @brief         memory allocate
 * @details       simulation af realloc function
 *                Can allocate only 3 buffers with sizes 8, 64 and 1028 bytes
 * @param[in]     ptr - pointer to memory
 * @param[in]     size - buffer size to allocate
 *
 * @return        pointer to memory or null
 */
void* TERM_gpio_realloc(void *ptr, size_t size);
/**
 * @brief         memory free
 * @details       simulation af free function
 * @param[in]     ptr - pointer to memory
 *
 * @return        nothing
 */
void TERM_gpio_free(void *ptr);
/** @} */
#endif //_TERMGPIO_H_
