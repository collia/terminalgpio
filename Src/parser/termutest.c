#include <stdio.h>
//#include <stdbool.h>
#include <string.h>
#include "termgpio.h"

#include "utest.h"

int TERM_parser();
void TERM_MOCK_init_input_buffer(const char* lines);
const char * TERM_MOCK_get_output_buffer();

#define TERM_UTEST_DEFAULT_GPIO_CONFIG           \
    {'A', 1, false, false, 0, 0},                     \
            {'B', 2, false, false, 0, 0},             \
            {'B', 3, true, false,  0, 0},               \
            {'A', 3, false, false, 0, 0},               \
            {0, 0, false, false, 0, 0},
#define TERM_UTEST_DEFAULT_TIM_CONFIG                           \
    {2, 50, {{'A', 0}, {'A', 1},{'A', 2},{'A', 3}}},            \
            {0, 0,  {{0, 0}, {0, 0},{0, 0},{0, 0}}}           


static bool test_positive_term(
        TERM_gpio_port_info_TYP *gpio_test_config,
        TERM_gpio_tim_pwm_info_TYP *pwm_test_config,
        const char* input,
        const char* waited_output)
{
    int rc = 0;
    const char* output;
    TERM_gpio_set_info(gpio_test_config, pwm_test_config);
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
        for(int i = 0; i < 100; i++)
        {
            printf("%03d: 0x%02hhx != 0x%02hhx %s\n", i, output[i], waited_output[i], ((output[i] != waited_output[i])?"!!!":" "));
        }
        return false;       
    }
    return true;
}

static bool test_negative_term(
        TERM_gpio_port_info_TYP *gpio_test_config,
        TERM_gpio_tim_pwm_info_TYP *pwm_test_config,
        const char* input,
        const char* waited_output)
{
    int rc = 0;
    const char* output;
    TERM_gpio_set_info(gpio_test_config, pwm_test_config);
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
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] ="help";
    char output[] = "\r\nHelp:\r\n"
        "\thelp|?\r\n"
        "\tgpio info\r\n"
        "\tpwm info\r\n"
        "\tgpio A|B|C|D port [0-15] mode on|off\r\n"
        "\tgpio A|B|C|D port [0-15] mode pwm [0-100]%\r\n"
        "\tpwm tim [1-3] freq <int>\r\n";

    PRINT_RESULT(test_positive_term(
        gpio_info,
        pwm_info,
        input,
        output));
    return result;
}

static bool test_help_2()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] ="?";
    char output[] = "\r\nHelp:\r\n"
        "\thelp|?\r\n"
        "\tgpio info\r\n"
        "\tpwm info\r\n"
        "\tgpio A|B|C|D port [0-15] mode on|off\r\n"
        "\tgpio A|B|C|D port [0-15] mode pwm [0-100]%\r\n"
        "\tpwm tim [1-3] freq <int>\r\n";

    PRINT_RESULT(test_positive_term(
        gpio_info,
        pwm_info,
        input,
        output));
    return result;
}


static bool test_info()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] ="gpio info\n";
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "A.3\toff\r\n";
    PRINT_RESULT(test_positive_term(
        gpio_info,
        pwm_info,
        input,
        output));
    return result;
}

static bool test_info_2()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] ="gpio info\n";
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "A.3\tpwm 34% tim2 50Hz\r\n";

    gpio_info[3].is_PWM = true;
    gpio_info[3].pwm_info = &pwm_info[0];
    gpio_info[3].duty = 34;
    
    PRINT_RESULT(test_positive_term(
        gpio_info,
        pwm_info,
        input,
        output));
    return result;
}

static bool test_pwm_info()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] ="pwm info\n";
    char output[] = "\r\nTIM2 Freq 50Hz:\r\n"
        "\tA.1\tpwm off\r\n"
        "\tA.3\tpwm 34%\r\n";

    gpio_info[3].is_PWM = true;
    gpio_info[3].pwm_info = &pwm_info[0];
    gpio_info[3].duty = 34;
    
    PRINT_RESULT(test_positive_term(
        gpio_info,
        pwm_info,
        input,
        output));
    return result;
}


static bool test_set_on()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] =  "gpio a port 1 mode on\n"
        "gpio info\n";
    char output[] = 
        "\r\nA.1\ton\r\n"
        "\r\nA.1\ton\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "A.3\toff\r\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                pwm_info,
                input,
                output));
    return result;
}

static bool test_set_off()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] ="gpio B port 3 mode off\n"
        "gpio info\n";
    char output[] = "\r\nB.3\toff\r\n"
        "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\toff\r\n"
        "A.3\toff\r\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                pwm_info,
                input,
                output));
    return result;
}

static bool test_set_pwm_on()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] ="gpio A port 3 mode pwm 10%\n"
        "gpio info\n";
    char output[] =  "\r\nA.3\tpwm 10% tim2 50Hz\r\n"
        "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "A.3\tpwm 10% tim2 50Hz\r\n";
    PRINT_RESULT(test_positive_term(
                gpio_info,
                pwm_info,
                input,
                output));
    return result;

}

static bool test_set_pwm_off()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] ="gpio info\n"
        "gpio A port 3 mode on\n"
        "gpio info\n";
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "A.3\tpwm 34% tim2 50Hz\r\n"
        "\r\nA.3\ton\r\n"
        "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "A.3\ton\r\n";

    gpio_info[3].is_PWM = true;
    gpio_info[3].pwm_info = &pwm_info[0];
    gpio_info[3].duty = 34;
    
    PRINT_RESULT(test_positive_term(
                gpio_info,
                pwm_info,
                input,
                output));
    return result;
}

static bool test_set_pwm_freq()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] ="gpio info\n"
        "pwm tim 2 freq 101\n"
        "gpio info\n";
    char output[] = "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "A.3\tpwm 34% tim2 50Hz\r\n"
        "\r\nTIM2 Freq 101Hz:\r\n"
        "\tA.1\tpwm off\r\n"
        "\tA.3\tpwm 34%\r\n"
        "\r\nA.1\toff\r\n"
        "B.2\toff\r\n"
        "B.3\ton\r\n"
        "A.3\tpwm 34% tim2 101Hz\r\n";

    gpio_info[3].is_PWM = true;
    gpio_info[3].pwm_info = &pwm_info[0];
    gpio_info[3].duty = 34;
    
    PRINT_RESULT(test_positive_term(
                gpio_info,
                pwm_info,
                input,
                output));
    return result;
}


static bool test_error_incorrect_port()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] = "gpio a port 10 mode on\n"
        "gpio info\n";
    char output[] = "\r\nError: Port is not allowed\r\n";
    PRINT_RESULT(test_negative_term(
                gpio_info,
                pwm_info,
                input,
                output));
    return result;
}

static bool test_error_incorrect_gpio()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] = "gpio F port 16 mode on\n"
        "gpio info\n";
    char output[] = "\r\nError: syntax error\r\n";
    PRINT_RESULT(test_negative_term(
                gpio_info,
                pwm_info,
                input,
                output));
    return result;
}

static bool test_error_incorrect_pwm()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] = "gpio B port 2 mode pwm 10%\n"
        "gpio info\n";
    char output[] = "\r\nError: PWM is not allowed on this port\r\n";
    PRINT_RESULT(test_negative_term(
                gpio_info,
                pwm_info,
                input,
                output));
    return result;
}

static bool test_error_incorrect_pwm_persent()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] = "gpio A port 1 mode pwm 101%\n"
        "gpio info\n";
    char output[] = "\r\nError: incorrect persent\r\n";
    PRINT_RESULT(test_negative_term(
                gpio_info,
                pwm_info,
                input,
                output));
    return result;
}

static bool test_error_incorrect_pwm_tim()
{
    bool result = true;
    TERM_gpio_port_info_TYP gpio_info[10] =
        {
            TERM_UTEST_DEFAULT_GPIO_CONFIG
        };
    TERM_gpio_tim_pwm_info_TYP pwm_info[10] =
        {
            TERM_UTEST_DEFAULT_TIM_CONFIG
        };
    char input[] = "pwm tim 1 freq 101\n"
        "gpio info\n";
    char output[] = "\r\nError: tim is not allowed\r\n";
    PRINT_RESULT(test_negative_term(
                gpio_info,
                pwm_info,
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
    rc &= test_info_2();
    rc &= test_pwm_info();
    rc &= test_set_on();
    rc &= test_set_off();
    rc &= test_set_pwm_on();
    rc &= test_set_pwm_off();
    rc &= test_set_pwm_freq();
    rc &= test_error_incorrect_port();
    rc &= test_error_incorrect_gpio();
    rc &= test_error_incorrect_pwm();
    rc &= test_error_incorrect_pwm_persent();
    rc &= test_error_incorrect_pwm_tim();

    return rc;
}

