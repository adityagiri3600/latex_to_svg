#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

int main() {
    char input[256];
    
    while (fgets(input, sizeof(input), stdin)) {
        if (strncmp(input, "exit", 4) == 0) {
            break;
        }
        
        char* result = latex_to_svg_string(input);
        if (result) {
            printf("%s", result);
            free(result);
        }
    }

    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
