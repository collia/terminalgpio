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
       TERM_debug_print("Error: ");
       TERM_debug_print(str);
       TERM_debug_print("\r");
       TERM_debug_print("\n");
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

%token TOKGPIO TOKINFO TOKPORT TOKMODE TOKPWM TOKFREQ TOKHELP

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
    gpio_info | gpio_mode | gpio_pwm | help

gpio_info:
    TOKGPIO TOKINFO
    {
            TERM_gpio_port_info_TYP * data;
    
            data = TERM_gpio_get_info();
            if(data == 0)
            {
                return -1;
            }
            while(data->port != 0 && data->line != 0)
            {
                if (TERM_gpio_print_port_info(data) < 0)
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
        port = TERM_gpio_set_mode($2, $4, $6, false, 0, 0);
        if(port <= 0) {
            return -1;
        }
        TERM_gpio_print_port_info(port);
    }
    ;

gpio_pwm:
    TOKGPIO PORT TOKPORT NUMBER TOKMODE TOKPWM TOKFREQ NUMBER PERCENT
    {
        TERM_gpio_port_info_TYP * port;
        port = TERM_gpio_set_mode($2, $4, false, true, $8, $9);
        if(port <= 0) {
            return -1;
        }
        TERM_gpio_print_port_info(port);
    }
    ;
help:
    TOKHELP
    {
            TERM_debug_print("Help:\r\n"
                             "\tgpio info\r\n"
                             "\tgpio A|B|C|D port [1-16] mode on|off\r\n"
                             "\tgpio A|B|C|D port [1-16] mode pwm freq <Int> [0-100]%\r\n");

    }   
