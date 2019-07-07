%{
#include <stdio.h>
#include <string.h>

int yylex();
int yy_scan_string(const char*);
int yyparse(void);

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	return 1;
}

int main()
{
    int rc;
    char input[] = "gpio C port 16 mode on\n"
        "gpio info\n"
        "gpio A port 21 mode pwm freq 10 10% \n"
        "gpio c port 16 mode off\n"
        "gpio a port 0 mode off\n";

    /*Copy string into new buffer and Switch buffers*/
    yy_scan_string (input);
    rc = yyparse();
    printf(" rc=%d\n", rc);
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
        printf("\tGPIO INFO\n");
		
	}
	;

gpio_mode:
	TOKGPIO PORT TOKPORT NUMBER TOKMODE STATE
	{
		printf("\tPort %d.%d state %d \n",$2, $4, $6);
	}
	;

gpio_pwm:
	TOKGPIO PORT TOKPORT NUMBER TOKMODE TOKPWM TOKFREQ NUMBER PERCENT
	{
		printf("\tPort %d.%d state pwm %d %d%% \n",$2, $4, $8, $9);
	}
	;
