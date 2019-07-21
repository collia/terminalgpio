%option noyyalloc noyyfree noyyrealloc
%option nostdinit
%option nounput
%{
//#include <stdio.h>
//#include <string.h>
#include "y.tab.h"
#include "termgpio.h"

#define YY_NO_UNISTD_H
#define YY_NEED_STRLEN
#define YY_USE_CUSTOM_ALLOCATOR
#define YY_BUF_SIZE 1024

char TERM_get_input_buf(char* buf, int max_size);
void Error_Handler(void);

#define YY_INPUT(buf,result,max_size) result = TERM_get_input_buf(buf, max_size);
#define YY_FATAL_ERROR(msg) {TERM_debug_print("%s\r\n", msg);Error_Handler();}

void* yyalloc(size_t size) { return TERM_gpio_malloc(size);}
void* yyrealloc(void* ptr, size_t size) { return TERM_gpio_realloc(ptr, size);}
void yyfree(void* ptr) {TERM_gpio_free(ptr);}

#define ECHO 
#define YY_MEMSET TERM_gpio_memset

%}
%%
[0-9]+                  yylval.number=TERM_gpio_atoi(yytext); return NUMBER;
help                    return TOKHELP;
\?                      return TOKHELP;
gpio                    return TOKGPIO;
info                    return TOKINFO;
port                    return TOKPORT;
mode                    return TOKMODE;
on|off                  yylval.number=!TERM_gpio_strcmp(yytext,"on"); return STATE;
pwm                     return TOKPWM;
tim                     return TOKTIM;
freq                    return TOKFREQ;
[0-9]+%                 yylval.number=TERM_gpio_atoi(yytext); return PERCENT;
[A|B|C|D]               yylval.number=yytext[0]-'A'; return PORT;
[a|b|c|d]               yylval.number=yytext[0]-'a'; return PORT;
\n                      /* ignore end of line */;
[ \t]+                  /* ignore whitespace */;

%%

