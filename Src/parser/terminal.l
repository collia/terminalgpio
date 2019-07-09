%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"


char TERM_get_input_buf(char* buf, int max_size);

#define YY_INPUT(buf,result,max_size) result = TERM_get_input_buf(buf, max_size);

//    { \
    int c = getchar() ; \
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }

%}
%%
[0-9]+                  yylval.number=atoi(yytext); return NUMBER;
help                    return TOKHELP;
gpio                    return TOKGPIO;
info                    return TOKINFO;
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

