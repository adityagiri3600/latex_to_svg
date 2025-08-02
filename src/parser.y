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
    int num;
    char* str;
    Node* node;
}

%token <num> NUMBER
%token <str> VARIABLE
%token FRAC PLUS MINUS MULTIPLY DIVIDE LPAREN RPAREN LBRACE RBRACE

%type <node> expression program

%start program

%%

program:
    expression { result_ast = $1; }
    ;

expression:
    NUMBER                            { $$ = create_number_node($1); }
    | VARIABLE                        { $$ = create_variable_node($1); }
    | expression PLUS expression      { $$ = create_binary_op_node('+', $1, $3); }
    | expression MINUS expression     { $$ = create_binary_op_node('-', $1, $3); }
    | expression MULTIPLY expression  { $$ = create_binary_op_node('*', $1, $3); }
    | expression DIVIDE expression    { $$ = create_binary_op_node('/', $1, $3); }
    | LPAREN expression RPAREN        { $$ = $2; }
    | FRAC LBRACE expression RBRACE LBRACE expression RBRACE { $$ = create_fraction_node($3, $6); }
    ;

%%
