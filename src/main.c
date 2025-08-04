#include <stdio.h>
#include <string.h>
#include "ast.h"

extern int yyparse();
extern void yy_scan_string(const char*);
extern Node* result_ast;

int main() {
    char input[256];

    // printf("LaTeX to SVG Compiler\n");
    // printf("Enter expression (or 'exit'): ");
    
    while (fgets(input, sizeof(input), stdin)) {
        if (strncmp(input, "exit", 4) == 0) {
            break;
        }
        
        yy_scan_string(input);
        if (yyparse() == 0 && result_ast) {
            calculate_dimensions(result_ast);
            // printf("\n--- AST Structure ---\n");
            // print_ast(result_ast, 0);
            // printf("--- End AST ---\n\n");
            
            printf("<svg width=\"%d\" height=\"150\">\n", result_ast->width + 40);
            printf("<defs>\n"
                   "    <style type=\"text/css\"><![CDATA[\n"
                   "    @font-face{\n"
                   "        font-family:\"LM Roman\";\n"
                   "        src:url(\"LMRoman10-Regular.otf\") format(\"opentype\");\n"
                   "    }\n"
                   "    text{ font-family:\"LM Roman\", serif; }\n"
                   "    ]]></style>\n"
                   "</defs>\n");

            generate_svg(result_ast, 20, 75, 1.0);
            
            printf("</svg>\n");
            free_ast(result_ast);
            result_ast = NULL;
        }
        // printf("\nEnter expression (or 'exit'): ");
    }

    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
