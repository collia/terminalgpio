%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
%}
%%
[0-9]+                  yylval.number=atoi(yytext); return NUMBER;
gpio		            return TOKGPIO;
info		            return TOKINFO;
port                    return TOKPORT;
mode                    return TOKMODE;
on|off                  yylval.number=!strcmp(yytext,"on"); return STATE;
pwm                     return TOKPWM;
freq                    return TOKFREQ;
[0-9]+%                 yylval.number=atoi(yytext); return PERCENT;
[A|B|C|D]               yylval.number=yytext[0]-'A'; return PORT;
[a|b|c|d]               yylval.number=yytext[0]-'a'; return PORT;
\n                      /* ignore end of line */;
[ \t]+                  /* ignore whitespace */;
%%

/*Implementationofinputfunction*/
int pipe_yyinput(char *buf,int max_size)
{
/*Readfromserialporttogetinput*/
/*Returnnumberofcharsreceived*/
    char input[] = "gpio C port 16 mode on\n"
        "gpio info\n"
        "gpio A port 21 mode pwm freq 10 10% \n"
        "gpio c port 16 mode off\n"
        "gpio a port 0 mode off\n";
    memcpy(buf, input, max_size);
    printf("yyinput %d\n", max_size);
    return max_size;
}
