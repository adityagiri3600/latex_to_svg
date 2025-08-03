%{
#include "parser.h"
#include "ast.h"
#include <stdio.h>

#define DEBUG_TOKENS 0
%}

%option noyywrap yylineno

%%

"^"             { if(DEBUG_TOKENS) fprintf(stderr, "TOKEN: CARET\n"); return CARET; }
"_"             { if(DEBUG_TOKENS) fprintf(stderr, "TOKEN: UNDERSCORE\n"); return UNDERSCORE; }
"\\frac"        { if(DEBUG_TOKENS) fprintf(stderr, "TOKEN: FRAC\n"); return FRAC; }
"{"             { if(DEBUG_TOKENS) fprintf(stderr, "TOKEN: LBRACE\n"); return LBRACE; }
"}"             { if(DEBUG_TOKENS) fprintf(stderr, "TOKEN: RBRACE\n"); return RBRACE; }
[ \t\n]+        { if(DEBUG_TOKENS) fprintf(stderr, "TOKEN: WHITESPACE (ignored)\n"); /* ignore whitespace */ }
.               { if(DEBUG_TOKENS) fprintf(stderr, "TOKEN: CHARACTER '%c'\n", yytext[0]); yylval.ch = yytext[0]; return CHARACTER; }

%%
