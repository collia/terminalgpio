#include <stdio.h>
//#include <stdbool.h>
#include <string.h>
#include "termgpio.h"

int TERM_parser();
void TERM_MOCK_init_input_buffer(const char* lines);
const char * TERM_MOCK_get_output_buffer();


#define PRINT_RESULT(a) if(a) {printf("%s \t\t PASS\n", __func__);}else{printf("%s \t\t FAIL\n", __func__);}

static bool test_positive_term(
        TERM_gpio_port_info_TYP *gpio_test_config,
        const char* input,
        const char* waited_output)
{
    int rc = 0;
    const char* output;
    TERM_gpio_set_info(gpio_test_config);
    TERM_MOCK_init_input_buffer(input);
    rc = TERM_parser();
    output = TERM_MOCK_get_output_buffer();
    if(rc != 0)
    {
        printf("Test failed. rc = %d\n",rc);
        printf("Output:\n%s", output);
        return false;
    }
    if(rc = strcmp(output, waited_output))
    {
        printf("Test failed. char = %d\n%s\n!=\n%s\n", rc, output, waited_output);
        return false;       
    }
    return true;

}


static bool test_negative_term(
        TERM_gpio_port_info_TYP *gpio_test_config,
        const char* input,
        const char* waited_output)
{
    int rc = 0;
    const char* output;
    TERM_gpio_set_info(gpio_test_config);
    TERM_MOCK_init_input_buffer(input);
    rc = TERM_parser();
    output = TERM_MOCK_get_output_buffer();
    if(rc == 0)
    {
        printf("Test failed. rc = %d\n",rc);
        printf("Output:\n%s", output);
        return false;
    }
    if(rc = strcmp(output, waited_output))
    {
        printf("Test failed. char = %d\n%s\n!=\n%s\n", rc, output, waited_output);
        return false;       
    }
    return true;

}



static int test_help()
{
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] ="help\n";
    char output[] = "Help:\n"
    "\tgpio info\n"
    "\tgpio A|B|C|D port [1-16] mode on|off\n"
    "\tgpio A|B|C|D port [1-16] mode pwm freq <Int> [0-100]%\n";

    PRINT_RESULT(test_positive_term(
        gpio_info,
        input,
        output));
}


static int test_info()
{
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] ="gpio info\n";
    char output[] = "A.1\toff\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\t50 Hz 50%\n"; 
    PRINT_RESULT(test_positive_term(
        gpio_info,
        input,
        output));
}

static int test_set_on()
{
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] = "gpio info\n"
        "gpio a port 1 mode on\n"
        "gpio info\n";
    char output[] = "A.1\toff\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\t50 Hz 50%\n"
        "Port 0.1 state 1\n"
        "A.1\ton\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\t50 Hz 50%\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                input,
                output));
}

static int test_set_off()
{
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] ="gpio info\n"
        "gpio B port 3 mode off\n"
        "gpio info\n";
    char output[] = "A.1\toff\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\t50 Hz 50%\n"
        "A.1\ton\n"
        "A.1\toff\n"
        "B.2\toff\n"
        "B.3\toff\n"
        "B.4\t50 Hz 50%\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                input,
                output));
}

static int test_set_pwm_on()
{
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] ="gpio info\n"
        "gpio A port 1 mode pwm freq 20 10%\n"
        "gpio info\n";
    char output[] = "A.1\toff\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\t50 Hz 50%\n"
        "Port 0.1 state pwm 20 10%\n"
        "A.1\t20 Hz 10%\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\t50 Hz 50%\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                input,
                output));

}

static int test_set_pwm_off()
{
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] ="gpio info\n"
        "gpio B port 4 mode on\n"
        "gpio info\n";
    char output[] = "A.1\toff\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\t50 Hz 50%\n"
        "Port 1.4 state 1\n"
        "A.1\toff\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\ton\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                input,
                output));
}

static int test_error_incorrect_port()
{
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] = "gpio info\n"
        "gpio a port 16 mode on\n"
        "gpio info\n";
    char output[] = "A.1\toff\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\t50 Hz 50%\n"
        "Port 0.16 state 1\n"
        "Error: Port is not allowed\n";
    PRINT_RESULT(test_negative_term(
                gpio_info,
                input,
                output));
}

static int test_error_incorrect_gpio()
{
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] = "gpio info\n"
        "gpio F port 16 mode on\n"
        "gpio info\n";
    char output[] = "A.1\toff\n"
        "B.2\toff\n"
        "B.3\ton\n"
        "B.4\t50 Hz 50%\n"
        "Error: syntax error\n";
    PRINT_RESULT(test_negative_term(
                gpio_info,
                input,
                output));
}


int main()
{
        
    test_help();
    test_info();
    test_set_on();
    test_set_off();
    test_set_pwm_on();
    test_set_pwm_off();
    test_error_incorrect_port();
    test_error_incorrect_gpio();
}
