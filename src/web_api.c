#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h>
#include "ast.h"

extern int yyparse();
extern void yy_scan_string(const char*);
extern Node* result_ast;

EMSCRIPTEN_KEEPALIVE
char* latex_to_svg(const char* latex_input) {
    if (!latex_input) return NULL;
    
    yy_scan_string(latex_input);
    if (yyparse() == 0 && result_ast) {
        calculate_dimensions(result_ast);
        
        // Calculate total SVG size
        int total_size = 1000; // Estimate
        char* svg_output = malloc(total_size);
        if (!svg_output) return NULL;
        
        // Build SVG
        snprintf(svg_output, total_size,
            "<svg width=\"%d\" height=\"150\">\n"
            "<defs>\n"
            "    <style type=\"text/css\"><![CDATA[\n"
            "    text{ font-family: \"Times New Roman\", serif; font-size: 16px; }\n"
            "    ]]></style>\n"
            "</defs>\n",
            result_ast->width + 40
        );
        
        char* svg_content = generate_svg(result_ast, 20, 75, 1.0);
        if (svg_content) {
            strncat(svg_output, svg_content, total_size - strlen(svg_output) - 1);
            free(svg_content);
        }
        
        strncat(svg_output, "</svg>", total_size - strlen(svg_output) - 1);
        
        free_ast(result_ast);
        result_ast = NULL;
        
        return svg_output;
    }
    
    return NULL;
}

EMSCRIPTEN_KEEPALIVE
void free_result(char* ptr) {
    if (ptr) free(ptr);
}