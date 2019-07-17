#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "../../Inc/ringbuffer.h"


RING_BUFFER_DECL(test, 10)


#define PRINT_RESULT(a) if(a) {printf("%s \t\t PASS\n", __func__);}else{printf("%s \t\t FAIL\n", __func__);}

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



static void test_ring_buffer_clear(){
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));

    PRINT_RESULT(check_buffer(RING_BUFFER(test), "aaaaaaaaaa", 10));
}

static void test_ring_buffer_insert(){
    char buffer[]="hello";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "helloaaaaa", 10));
}

static void test_ring_buffer_insert2(){
    char buffer[]="hey";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "heyheyaaaa", 10));
}

static void test_ring_buffer_insert3(){
    char buffer[]="123456";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "1234561234", 10));
}

static void test_ring_buffer_insert_too_many_1(){
    char buffer[]="123456789abc";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    //RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "123456789a", 10));
}

static void test_ring_buffer_insert_too_many_2(){
    char buffer1[]="123456789abc";
    char buffer2[]="ABCD";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer1, sizeof(buffer1)-1);
    RING_BUFFER_INSERT(test, buffer2, sizeof(buffer2)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "123456789a", 10));
}

static void test_ring_buffer_insert_too_many_3(){
    char buffer1[]="123456";
    char buffer2[]="ABCDEF";
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer1, sizeof(buffer1)-1);
    RING_BUFFER_INSERT(test, buffer2, sizeof(buffer2)-1);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "123456ABCD", 10));
}

static void test_ring_buffer_get_1(){
    char *result;
    int len = 0;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_GET(test, result, len)
    PRINT_RESULT(len == 0);
}

static void test_ring_buffer_get_2(){
    char buffer[]="hey";
    char *result;
    int len;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_GET(test, result, len)

    PRINT_RESULT(check_buffer(result, "hey", len));
}

static void test_ring_buffer_insert_get_1(){
    char buffer[]="hey";
    char *result;
    int len;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_GET(test, result, len)

    PRINT_RESULT(check_buffer(result, "heyhey", len));
}


static void test_ring_buffer_insert_get_2(){
    char buffer[]="123456";
    char buffer2[]="ABCCBA";
    char *result;
    int len;
    memset(RING_BUFFER(test), 'a', sizeof(RING_BUFFER(test)));
    RING_BUFFER_RESET(test);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_INSERT(test, buffer, sizeof(buffer)-1);
    RING_BUFFER_GET(test, result, len);

    PRINT_RESULT(len != 0);
    PRINT_RESULT(check_buffer(result, "1234561234", len));

    RING_BUFFER_INSERT(test, buffer2, sizeof(buffer)-1);
    RING_BUFFER_GET(test, result, len);
    PRINT_RESULT(len != 0);
    PRINT_RESULT(check_buffer(result, "ABCCBA1234", len));
    RING_BUFFER_INSERT(test, buffer2, sizeof(buffer)-1);
    PRINT_RESULT(len != 0);
    PRINT_RESULT(check_buffer(RING_BUFFER(test), "ABCCBACCBA", 10));
}

void test_ring_buffer() {
    test_ring_buffer_clear();
    test_ring_buffer_insert();
    test_ring_buffer_insert2();
    test_ring_buffer_insert3();
    test_ring_buffer_insert_too_many_1();
    test_ring_buffer_insert_too_many_2();
    test_ring_buffer_insert_too_many_3();
    test_ring_buffer_get_1();
    test_ring_buffer_get_2();
    test_ring_buffer_insert_get_1();
    test_ring_buffer_insert_get_2();
}

