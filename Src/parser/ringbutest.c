#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "../../Inc/ringbuffer.h"
#include "utest.h"

RING_BUFFER_DECL(test, 10)

static bool check_buffer(
        const char* buffer,
        const char* waited_content,
        int len)
{
    int rc = 0;
    if(rc = memcmp(buffer, waited_content, len))
    {
        printf("Test failed. char = %d\n%s\n!=\n%s\n", rc, buffer, waited_content );
        printf("Test failed. In=%d Out=%d\n", RING_BUFFER_IN_PTR(test), RING_BUFFER_OUT_PTR(test));
        return false;       
    }
    return true;

}



static bool test_ring_buffer_clear(){
    bool result = true;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));

    PRINT_RESULT(check_buffer(RING_BUFFER(test), "aaaaaaaaaa", 10));

    return result;
}

static bool test_ring_buffer_insert(){
    bool result = true;
    char buffer[]="hello";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "helloaaaaa", 10));

    return result;
}

static bool test_ring_buffer_insert2(){
    bool result = true;
    char buffer[]="hey";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "heyheyaaaa", 10));
    return result;
}

static bool test_ring_buffer_insert3(){
    bool result = true;
    char buffer[]="123456";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "1234561234", 10));

    return result;
}

static bool test_ring_buffer_insert_too_many_1(){
    bool result = true;
    char buffer[]="123456789abc";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    //RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "123456789a", 10));
    return result;
}

static bool test_ring_buffer_insert_too_many_2(){
    bool result = true;
    char buffer1[]="123456789abc";
    char buffer2[]="ABCD";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer1, sizeof(buffer1)-1);
    RING_BUFFER_INSERT(test, buffer2, sizeof(buffer2)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "123456789a", 10));
    return result;
}

static bool test_ring_buffer_insert_too_many_3(){
    bool result = true;
    char buffer1[]="123456";
    char buffer2[]="ABCDEF";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer1, sizeof(buffer1)-1);
    RING_BUFFER_INSERT(test, buffer2, sizeof(buffer2)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "123456ABCD", 10));
    return result;
}

static bool test_ring_buffer_get_1(){
    bool result = true;
    char *res;
    int len = 0;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_GET(test, res, len)
    PRINT_RESULT(len == 0);
    return result;
}

static bool test_ring_buffer_get_2(){
    bool result = true;
    char buffer[]="hey";
    char *res;
    int len;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_GET(test, res, len)

    PRINT_RESULT(check_buffer(res, "hey", len));
    return result;
}

static bool test_ring_buffer_insert_get_1(){
    bool result = true;
    char buffer[]="hey";
    char *res;
    int len;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_GET(test, res, len)

    PRINT_RESULT(check_buffer(res, "heyhey", len));
    return result;
}


static bool test_ring_buffer_insert_get_2(){
    bool result = true;
    char buffer[]="123456";
    char buffer2[]="ABCCBA";
    char *res;
    int len=20;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_GET(test, res, len);

    PRINT_RESULT(len != 0);
    PRINT_RESULT(check_buffer(res, "1234561234", len));

    RING_BUFFER_INSERT(test, buffer2, sizeof(buffer2)-1);
    RING_BUFFER_GET(test, res, len);
    PRINT_RESULT(len != 0);
    PRINT_RESULT(check_buffer(res, "ABCCBA1234", len));
    RING_BUFFER_INSERT(test, buffer2, sizeof(buffer2)-1);

    PRINT_RESULT(check_buffer(RING_BUFFER(test), "ABCCBACCBA", 10));
    return result;
}

static bool test_ring_buffer_insert_get_max_len_1(){
    bool result = true;
    char buffer[]="1234";
    char *res;
    int len=2;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);

    RING_BUFFER_GET(test, res, len);

    PRINT_RESULT(len ==2 );
    PRINT_RESULT(check_buffer(res, "12", len));

    RING_BUFFER_GET(test, res, len);
    PRINT_RESULT(len == 2);
    PRINT_RESULT(check_buffer(res, "34", len));

    RING_BUFFER_GET(test, res, len);
    PRINT_RESULT(len == 0);
    return result;
}

bool test_ring_buffer() {
    bool rc = true;
    rc &= test_ring_buffer_clear();
    rc &= test_ring_buffer_insert();
    rc &= test_ring_buffer_insert2();
    rc &= test_ring_buffer_insert3();
    rc &= test_ring_buffer_insert_too_many_1();
    rc &= test_ring_buffer_insert_too_many_2();
    rc &= test_ring_buffer_insert_too_many_3();
    rc &= test_ring_buffer_get_1();
    rc &= test_ring_buffer_get_2();
    rc &= test_ring_buffer_insert_get_1();
    rc &= test_ring_buffer_insert_get_2();
    rc &= test_ring_buffer_insert_get_max_len_1();
    return rc;
}

