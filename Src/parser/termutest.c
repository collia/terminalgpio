#include <stdio.h>

int TERM_parser();
void TERM_MOCK_init_input_buffer(const char* lines);


static int test_help()
{
    int rc; 
    char input[] ="help\n";
    TERM_MOCK_init_input_buffer(input);
    rc = TERM_parser();
    printf("%s rc = %d\n", __func__, rc);
}


static int test_info()
{
    int rc; 
    char input[] ="gpio info\n";

    TERM_MOCK_init_input_buffer(input);
    rc = TERM_parser();
    printf("%s rc = %d\n", __func__, rc);
}

static int test_set_on()
{
    int rc; 
    char input[] ="gpio a port 1 mode on\n";
	    //"gpio info\n"
        //"gpio a port 1 mode on\n"
        //"gpio info\n";

    TERM_MOCK_init_input_buffer(input);
    rc = TERM_parser();
    printf("%s rc = %d\n", __func__, rc);
}

static int test_set_off()
{
    int rc; 
    char input[] ="gpio info\n"
        "gpio B port 3 mode off\n"
        "gpio info\n";

    TERM_MOCK_init_input_buffer(input);
    rc = TERM_parser();
    printf("%s rc = %d\n", __func__, rc);
}

static int test_set_pwm_on()
{
    int rc; 
    char input[] ="gpio info\n"
        "gpio A port 1 mode pwm freq 20 10%\n"
        "gpio info\n";

    TERM_MOCK_init_input_buffer(input);
    rc = TERM_parser();
    printf("%s rc = %d\n", __func__, rc);
}

static int test_set_pwm_off()
{
    int rc; 
    char input[] ="gpio info\n"
        "gpio B port 4 mode on\n"
        "gpio info\n";

    TERM_MOCK_init_input_buffer(input);
    rc = TERM_parser();
    printf("%s rc = %d\n", __func__, rc);
}

static int test_error_incorrect_port()
{
    int rc; 
    char input[] ="gpio info\n"
        "gpio E port 4 mode on\n"
        "gpio info\n";

    TERM_MOCK_init_input_buffer(input);
    rc = TERM_parser();
    printf("%s rc = %d\n", __func__, rc);
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
}
