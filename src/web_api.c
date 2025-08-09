#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h>
#include "ast.h"

static char* parse_error_message = NULL;

EMSCRIPTEN_KEEPALIVE
char* latex_to_svg(const char* input) {
    if (!input) return strdup("Error: No input provided");
    
    if (parse_error_message) {
        free(parse_error_message);
        parse_error_message = NULL;
    }
    
    char* result = latex_to_svg_string(input);
    
    if (result) {
        return result;
    } else if (parse_error_message) {
        char* error_svg = malloc(500);
        snprintf(error_svg, 500,
            "<svg width=\"400\" height=\"60\" xmlns=\"http://www.w3.org/2000/svg\">"
            "<text x=\"10\" y=\"20\" fill=\"red\" font-family=\"Arial\" font-size=\"14\">"
            "Parse Error: %s"
            "</text>"
            "</svg>",
            parse_error_message
        );
        return error_svg;
    } else {
        return strdup(
            "<svg width=\"300\" height=\"60\" xmlns=\"http://www.w3.org/2000/svg\">"
            "<text x=\"10\" y=\"20\" fill=\"orange\" font-family=\"Arial\" font-size=\"14\">"
            "Unknown parsing error"
            "</text>"
            "</svg>"
        );
    }
}

void yyerror(const char *s) {
    if (parse_error_message) {
        free(parse_error_message);
    }
    parse_error_message = strdup(s ? s : "Unknown error");
}