#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include "ast.h"

EMSCRIPTEN_KEEPALIVE
char* latex_to_svg(const char* input) {
    if (!input) return NULL;
    return latex_to_svg_string(input);
}

void yyerror(const char *s) {
    // pass
}