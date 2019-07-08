#include <stdio.h>

int TERM_parser();

static int test_info()
{
    int rc; 
    char input[] ="gpio info\n";

    rc = TERM_parser(input);
    printf("%s rc = %d\n", __func__, rc);
}

static int test_set_on()
{
    int rc; 
    char input[] ="gpio info\n"
        "gpio a port 1 mode on\n"
        "gpio info\n";

    rc = TERM_parser(input);
    printf("%s rc = %d\n", __func__, rc);
}

static int test_set_off()
{
    int rc; 
    char input[] ="gpio info\n"
        "gpio B port 3 mode off\n"
        "gpio info\n";

    rc = TERM_parser(input);
    printf("%s rc = %d\n", __func__, rc);
}

static int test_set_pwm_on()
{
    int rc; 
    char input[] ="gpio info\n"
        "gpio A port 1 mode pwm freq 20 10%\n"
        "gpio info\n";

    rc = TERM_parser(input);
    printf("%s rc = %d\n", __func__, rc);
}

static int test_set_pwm_off()
{
    int rc; 
    char input[] ="gpio info\n"
        "gpio B port 4 mode on\n"
        "gpio info\n";

    rc = TERM_parser(input);
    printf("%s rc = %d\n", __func__, rc);
}

static int test_error_incorrect_port()
{
    int rc; 
    char input[] ="gpio info\n"
        "gpio E port 4 mode on\n"
        "gpio info\n";

    rc = TERM_parser(input);
    printf("%s rc = %d\n", __func__, rc);
}

int main()
{
/*    char input[] = "gpio C port 16 mode on\n"
        "gpio info\n"
        "gpio A port 21 mode pwm freq 10 10% \n"
        "gpio c port 16 mode off\n"
        "gpio a port 0 mode off\n";

        TERM_parser(input);*/
    test_info();
    test_set_on();
    test_set_off();
    test_set_pwm_on();
    test_set_pwm_off();
    test_error_incorrect_port();

}
