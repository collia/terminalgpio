%{
#include <stdio.h>
#include <string.h>
#include "termgpio.h"

int yylex();
int yy_scan_string(const char*);
int yyparse(void);
int yylex_destroy(void);

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	return 1;
}

int TERM_parser(char * input)
{
    int rc;

    /*Copy string into new buffer and Switch buffers*/
    yy_scan_string (input);
    rc = yyparse();
    yylex_destroy();

    return rc;
}

char *heater="default";

%}

%token TOKGPIO TOKINFO TOKPORT TOKMODE TOKPWM TOKFREQ

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
	gpio_info | gpio_mode | gpio_pwm

gpio_info:
	TOKGPIO TOKINFO 
	{
        TERM_gpio_port_info_TYP * data;
        printf("GPIO INFO\n");
		data = TERM_gpio_info();
        while(data->port != 0 && data->line != 0)
        {
            if(data->is_PWM)
            {
                printf("%c.%d\t%d Hz %d%%\n", data->port, data->line, data->freq, data->duty);
            }
            else
            {
                printf("%c.%d\t %s\n", data->port, data->line, data->level?"on":"off");
            }
            data++;
        }
	}
	;

gpio_mode:
	TOKGPIO PORT TOKPORT NUMBER TOKMODE STATE
	{
		printf("\tPort %d.%d state %d \n",$2, $4, $6);
        if(TERM_gpio_set_mode($2, $4, $6, false, 0, 0) < 0)
        {
            return -1;
        }
	}
	;

gpio_pwm:
	TOKGPIO PORT TOKPORT NUMBER TOKMODE TOKPWM TOKFREQ NUMBER PERCENT
	{
		printf("\tPort %d.%d state pwm %d %d%% \n",$2, $4, $8, $9);
        if(TERM_gpio_set_mode($2, $4, false, true, $8, $9) < 0)
            return -1;
	}
	;
