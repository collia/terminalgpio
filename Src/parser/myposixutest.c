#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include <stdbool.h>

#include "termgpio.h"
#include "termutest.h"


static bool myposix_test_atoi()
{
    bool result = true;
    const char test_buf1[] = "123";
    PRINT_RESULT(TERM_gpio_atoi(test_buf1) == 123);

    const char test_buf2[] = "-192";
    PRINT_RESULT(TERM_gpio_atoi(test_buf2) == -192);

    const char test_buf3[] = "123ab";
    PRINT_RESULT(TERM_gpio_atoi(test_buf3) == 0);

    const char test_buf4[] = "123122312341123";
    PRINT_RESULT(TERM_gpio_atoi(test_buf4) == 0);

    const char test_buf5[] = "10";
    PRINT_RESULT(TERM_gpio_atoi(test_buf5) == 10);

    const char test_buf6[] = "7";
    PRINT_RESULT(TERM_gpio_atoi(test_buf6) == 7);

    const char test_buf7[] = "0";
    PRINT_RESULT(TERM_gpio_atoi(test_buf7) == 0);

    const char test_buf8[] = "10%";
    PRINT_RESULT(TERM_gpio_atoi(test_buf8) == 10);

    const char test_buf9[] = "10%00";
    PRINT_RESULT(TERM_gpio_atoi(test_buf9) == 10);

    return result;
}

static bool myposix_test_strcmp()
{
    bool result = true;
    
    const char test_buf1_1[] = "123";
    const char test_buf1_2[] = "123";
    
    PRINT_RESULT(TERM_gpio_strcmp(test_buf1_1, test_buf1_2) == 0);

    const char test_buf2_1[] = "12345678";
    const char test_buf2_2[] = "12344678";
    
    PRINT_RESULT(TERM_gpio_strcmp(test_buf2_1, test_buf2_2) == 5);

    const char test_buf3_1[] = "12345678";
    const char test_buf3_2[] = "1234\0678";
    
    PRINT_RESULT(TERM_gpio_strcmp(test_buf3_1, test_buf3_2) == 5);

    const char test_buf4_1[] = "12345678";
    const char test_buf4_2[] = "\012345678";
    
    PRINT_RESULT(TERM_gpio_strcmp(test_buf4_1, test_buf4_2) == 1);

    
    return result;
}

static bool myposix_test_itona()
{
    bool result = true;

    char buffer[10];
    memset(buffer, 0, 10);
    PRINT_RESULT(TERM_gpio_itona(123, buffer, 10) == 3); 
    PRINT_RESULT(memcmp(buffer, "123", 3)==0); 

    memset(buffer, 0, 10);
    PRINT_RESULT(TERM_gpio_itona(-123, buffer, 10) == 4); 
    PRINT_RESULT(memcmp(buffer, "-123", 4)==0); 

    memset(buffer, 0, 10);
    PRINT_RESULT(TERM_gpio_itona(-123, buffer, 0) == 0); 
    PRINT_RESULT(memcmp(buffer, "\0\0\0\0", 4)==0); 

    memset(buffer, 0, 10);
    PRINT_RESULT(TERM_gpio_itona(123, buffer, 2) == 2); 
    PRINT_RESULT(memcmp(buffer, "23\0\0", 4)==0); 

    memset(buffer, 0, 10);
    PRINT_RESULT(TERM_gpio_itona(-123, buffer, 2) == 2); 
    PRINT_RESULT(memcmp(buffer, "23\0\0", 4)==0); 

    memset(buffer, 0, 10);
    PRINT_RESULT(TERM_gpio_itona(0, buffer, 10) == 1); 
    PRINT_RESULT(memcmp(buffer, "0", 2)==0); 

    
    //printf("%s\n", buffer);
    return result;
}

bool myposix_utests()
{
    bool rc = true;
    rc &= myposix_test_atoi();
    rc &= myposix_test_strcmp();
    rc &= myposix_test_itona();
    return rc;
}
