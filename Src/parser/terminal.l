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
