#include <stdio.h>
//#include <stdbool.h>
#include <string.h>
#include "termgpio.h"

#include "utest.h"

int TERM_parser();
void TERM_MOCK_init_input_buffer(const char* lines);
const char * TERM_MOCK_get_output_buffer();


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



static bool test_help()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] ="help";
    char output[] = "\r\nHelp:\r\n"
        "\thelp|?\r\n"
        "\tgpio info\r\n"
        "\tgpio A|B|C|D port [0-15] mode on|off\r\n"
        "\tgpio A|B|C|D port [0-15] mode pwm freq <Int> [0-100]%\r\n";

    PRINT_RESULT(test_positive_term(
        gpio_info,
        input,
        output));
    return result;
}

static bool test_help_2()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] ="?";
    char output[] = "\r\nHelp:\r\n"
        "\thelp|?\r\n"
        "\tgpio info\r\n"
        "\tgpio A|B|C|D port [0-15] mode on|off\r\n"
        "\tgpio A|B|C|D port [0-15] mode pwm freq <Int> [0-100]%\r\n";

    PRINT_RESULT(test_positive_term(
        gpio_info,
        input,
        output));
    return result;
}


static bool test_info()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            {'A', 1, false, false, 0, 0},
            {'B', 2, false, false, 0, 0},
            {'B', 3, false, true, 0, 0},
            {'B', 4, true,  false, 50, 50},
            {0, 0, false, false, 0, 0},
        };
    char input[] ="gpio info\n";
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\t50 Hz 50%\r\n"; 
    PRINT_RESULT(test_positive_term(
        gpio_info,
        input,
        output));
    return result;
}

static bool test_set_on()
{
    bool result = true;
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
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\t50 Hz 50%\r\n"
        "\r\nA.1\ton\r\n"
        "\r\nA.1\ton\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\t50 Hz 50%\r\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                input,
                output));
    return result;
}

static bool test_set_off()
{
    bool result = true;
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
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\t50 Hz 50%\r\n"
        "\r\nB.3\toff\r\n"
        "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\toff\r\n"
        "B.4\t50 Hz 50%\r\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                input,
                output));
    return result;
}

static bool test_set_pwm_on()
{
    bool result = true;
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
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\t50 Hz 50%\r\n"
        "\r\nA.1\t20 Hz 10%\r\n"
        "\r\nA.1\t20 Hz 10%\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\t50 Hz 50%\r\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                input,
                output));
    return result;

}

static bool test_set_pwm_off()
{
    bool result = true;
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
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\t50 Hz 50%\r\n"
        "\r\nB.4\ton\r\n"
        "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\ton\r\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                input,
                output));
    return result;
}

static bool test_error_incorrect_port()
{
    bool result = true;
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
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\t50 Hz 50%\r\n"
        "\r\nError: Port is not allowed\r\n";
    PRINT_RESULT(test_negative_term(
                gpio_info,
                input,
                output));
    return result;
}

static bool test_error_incorrect_gpio()
{
    bool result = true;
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
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "B.4\t50 Hz 50%\r\n"
        "\r\nError: syntax error\r\n";
    PRINT_RESULT(test_negative_term(
                gpio_info,
                input,
                output));
    return result;
}


bool parser_utests()
{
    bool rc = true;
    rc &= test_help();
    rc &= test_help_2();
    rc &= test_info();
    rc &= test_set_on();
    rc &= test_set_off();
    rc &= test_set_pwm_on();
    rc &= test_set_pwm_off();
    rc &= test_error_incorrect_port();
    rc &= test_error_incorrect_gpio();

    return rc;
}

