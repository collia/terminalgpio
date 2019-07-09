%{
#include <stdio.h>
#include <string.h>
#include "termgpio.h"

#define TERM_PRINT_BUFFER_LENGTH 100

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
            char buffer[TERM_PRINT_BUFFER_LENGTH];
            TERM_gpio_port_info_TYP * data;
            printf("GPIO INFO\n");
	    data = TERM_gpio_info();
            while(data->port != 0 && data->line != 0)
            {
                if(data->is_PWM)
                {
                    snprintf(buffer, TERM_PRINT_BUFFER_LENGTH,"%c.%d\t%d Hz %d%%\n", data->port, data->line, data->freq, data->duty);
                    TERM_debug_print(buffer);
                }
                else
                {
                    snprintf(buffer, TERM_PRINT_BUFFER_LENGTH,"%c.%d\t %s\n", data->port, data->line, data->level?"on":"off");
                    TERM_debug_print(buffer);
                }
                data++;
        }
	}
	;

gpio_mode:
	TOKGPIO PORT TOKPORT NUMBER TOKMODE STATE
	{
            char buffer[TERM_PRINT_BUFFER_LENGTH];
	    snprintf(buffer, TERM_PRINT_BUFFER_LENGTH, "Port %d.%d state %d \n",$2, $4, $6);
            TERM_debug_print(buffer);
            if(TERM_gpio_set_mode($2, $4, $6, false, 0, 0) < 0)
            {
                return -1;
            }
	}
	;

gpio_pwm:
	TOKGPIO PORT TOKPORT NUMBER TOKMODE TOKPWM TOKFREQ NUMBER PERCENT
	{
            char buffer[TERM_PRINT_BUFFER_LENGTH];
            snprintf(buffer, TERM_PRINT_BUFFER_LENGTH, "Port %d.%d state pwm %d %d%% \n",$2, $4, $8, $9);
            TERM_debug_print(buffer);
            if(TERM_gpio_set_mode($2, $4, false, true, $8, $9) < 0)
                return -1;
	}
	;
help:
	TOKHELP
	{
            TERM_debug_print("Help:\n"
                             "\tgpio info\n"
                             "\tgpio A|B|C|D port [1-16] mode on|off\n"
                             "\tgpio A|B|C|D port [1-16] mode pwm freq <Int> [0-100]%\n");

	}	
