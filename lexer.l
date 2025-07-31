%{
#include "parser.h"
#include "ast.h"
%}

%option noyywrap yylineno

%%

[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
[a-zA-Z]+       { yylval.str = strdup(yytext); return VARIABLE; }
"\\frac"        { return FRAC; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULTIPLY; }
"/"             { return DIVIDE; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
[ \t\n]+        { /* ignore whitespace */ }
.               { yyerror("Invalid character"); }

%%
