%{
#include "parser.h"
#include "ast.h"
%}

%option noyywrap yylineno

%%

"\\frac"        { return FRAC; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
[ \t\n]+        { /* ignore whitespace */ }
.               { yylval.ch = yytext[0]; return CHARACTER; }

%%
