%{
//#include <stdio.h>
//#include <string.h>
#include "termgpio.h"


int yylex();
int yy_scan_string(const char*);
int yyparse(void);
int yylex_destroy(void);

void TERM_debug_print(const char *line);

void yyerror(const char *str)
{
       //fprintf(stderr,"error: %s\n",str);
       TERM_debug_print("\r\nError: ");
       TERM_debug_print(str);
       TERM_debug_print("\r\n");
}

int yywrap()
{
    return 1;
}

int TERM_parser()
{
    int rc;

    /*Copy string into new buffer and Switch buffers*/
    //yy_scan_string (input);
    rc = yyparse();
    //yylex_destroy();

    return rc;
}

char *heater="default";

%}

%token TOKGPIO TOKINFO TOKPORT TOKMODE TOKPWM TOKFREQ TOKHELP TOKTIM

%union
{
    int number;
}

%token <number> STATE
%token <number> NUMBER
%token <number> PERCENT
%token <number> PORT


%%

commands:
    | commands command
    ;


command:
    gpio_info | gpio_mode | gpio_pwm | pwm_info | pwm_freq | help

gpio_info:
    TOKGPIO TOKINFO
    {
            TERM_gpio_port_info_TYP * data;
            data = TERM_gpio_get_gpio_info();
            if(data == 0)
            {
                return -1;
            }
            TERM_debug_print("\r\n");
            while(!(data->idx.port == 0 && data->idx.line == 0))
            {
                if (TERM_gpio_print_port_info(data) < 0)
                {
                    return -1;
                }
                data++;
            }
    }
    ;
pwm_info:
    TOKPWM TOKINFO
    {
        TERM_gpio_tim_pwm_info_TYP *data;
        data = TERM_gpio_get_pwm_info();
        if(data == 0)
            {
                return -1;
            }
            TERM_debug_print("\r\n");
            while(data->tim != 0)
            {
                if (TERM_gpio_print_tim_info(data) < 0)
                {
                    return -1;
                }
                data++;
            }
    }
    ;

gpio_mode:
    TOKGPIO PORT TOKPORT NUMBER TOKMODE STATE
    {
        TERM_gpio_port_info_TYP * port;
        port = TERM_gpio_set_mode($2, $4, $6, false, 0);
        if(port <= 0) {
            return -1;
        }
        TERM_debug_print("\r\n");
        TERM_gpio_print_port_info(port);
    }
    ;

gpio_pwm:
    TOKGPIO PORT TOKPORT NUMBER TOKMODE TOKPWM PERCENT
    {
        TERM_gpio_port_info_TYP * port;
        port = TERM_gpio_set_mode($2, $4, false, true, $7);
        if(port <= 0) {
            return -1;
        }
        TERM_debug_print("\r\n");
        TERM_gpio_print_port_info(port);
    }
    ;
pwm_freq:
    TOKPWM TOKTIM NUMBER TOKFREQ NUMBER
    {
        TERM_gpio_tim_pwm_info_TYP * pwm;
        pwm = TERM_gpio_set_pwm_freq($3, $5);
        if(pwm <= 0) {
            return -1;
        }
        TERM_debug_print("\r\n");
        TERM_gpio_print_tim_info(pwm);
    }


help:
    TOKHELP
    {
            TERM_debug_print("\r\nHelp:\r\n"
                             "\thelp|?\r\n"
                             "\tgpio info\r\n"
                             "\tpwm info\r\n"
                             "\tgpio A|B|C|D port [0-15] mode on|off\r\n"
                             "\tgpio A|B|C|D port [0-15] mode pwm [0-100]%\r\n"
                             "\tpwm tim [1-3] freq <int>\r\n");

    }   
