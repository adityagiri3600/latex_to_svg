%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include "ast.h"

void yyerror(const char*);
int yylex(void);

Node* result_ast = NULL;
%}

%union {
    char ch;
    Node* node;
}

%token <ch> CHARACTER
%token FRAC LBRACE RBRACE

%type <node> expression program character_sequence frac_expr content

%start program

%%

program:
    expression { result_ast = $1; }
    ;

expression:
    character_sequence { $$ = $1; }
    | frac_expr { $$ = $1; }
    | expression expression { $$ = create_sequence_node($1, $2); }
    ;

character_sequence:
    CHARACTER { $$ = create_character_node($1); }
    | character_sequence CHARACTER { $$ = create_sequence_node($1, create_character_node($2)); }
    ;

frac_expr:
    FRAC LBRACE content RBRACE LBRACE content RBRACE { $$ = create_fraction_node($3, $6); }
    ;

content:
    expression { $$ = $1; }
    ;

%%