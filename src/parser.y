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
%token FRAC LBRACE RBRACE CARET UNDERSCORE

%type <node> expression program frac_expr superscript_expr subscript_expr content factor

%left '+' '-'  // Lower precedence
%left CARET UNDERSCORE  // Higher precedence

%start program

%%

program:
    expression { result_ast = $1; }
    ;

expression:
    factor { $$ = $1; }
    | expression factor { $$ = create_sequence_node($1, $2); }
    ;

factor:
    CHARACTER { $$ = create_character_node($1); }
    | frac_expr { $$ = $1; }
    | superscript_expr { $$ = $1; }
    | subscript_expr { $$ = $1; }
    ;

frac_expr:
    FRAC LBRACE content RBRACE LBRACE content RBRACE { $$ = create_fraction_node($3, $6); }
    ;

superscript_expr:
    factor CARET LBRACE content RBRACE { $$ = create_superscript_node($1, $4); }
    | factor CARET CHARACTER { $$ = create_superscript_node($1, create_character_node($3)); }
    ;

subscript_expr:
    factor UNDERSCORE LBRACE content RBRACE { $$ = create_subscript_node($1, $4); }
    | factor UNDERSCORE CHARACTER { $$ = create_subscript_node($1, create_character_node($3)); }
    ;

content:
    expression { $$ = $1; }
    ;

%%