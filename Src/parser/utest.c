#include <stdio.h>
//#include <stdbool.h>
#include <string.h>
#include "termgpio.h"

#include "utest.h"


extern bool parser_utests();
extern bool test_ring_buffer();
extern bool myposix_utests();

int main()
{
    bool rc = true;
    rc &= parser_utests();
    rc &= test_ring_buffer();
    rc &= myposix_utests();
    if(rc)
    {
        printf("OK\n");
        return 0;
    }
    else
    {
        printf("FAIL\n");
        return 1;
    }
}
