#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "svg_letters.h"


static char* string_append(char* str, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);
    
    if (len < 0) return str;
    size_t current_len = str ? strlen(str) : 0;
    str = realloc(str, current_len + len + 1);
    
    if (!str) return NULL;
    va_start(args, format);
    vsnprintf(str + current_len, len + 1, format, args);
    va_end(args);
    
    return str;
}

const int FONT_SIZE = 28; 
const int CHAR_WIDTH = 12;
const int CHAR_HEIGHT = 20;
const int PADDING = 10;
const int FRAC_PADDING = 5;    
const int CHAR_SPACING = 5;    
const float FRAC_SCALE = 0.8f; 
const float SCRIPT_SCALE = 0.6f; 

Node *create_character_node(char ch)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NODE_CHARACTER;
    node->data.character.ch = ch;
    node->width = CHAR_WIDTH;
    node->height = CHAR_HEIGHT;
    return node;
}

Node *create_sequence_node(Node *left, Node *right)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NODE_SEQUENCE;
    node->data.sequence.left = left;
    node->data.sequence.right = right;
    node->width = 0;
    node->height = 0;
    return node;
}

Node *create_fraction_node(Node *numerator, Node *denominator)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NODE_FRAC;
    node->data.fraction.numerator = numerator;
    node->data.fraction.denominator = denominator;
    node->width = 0;
    node->height = 0;
    return node;
}

Node *create_superscript_node(Node *base, Node *superscript)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NODE_SUPERSCRIPT;
    node->data.superscript.base = base;
    node->data.superscript.sup = superscript;
    node->width = 0;
    node->height = 0;
    return node;
}

Node *create_subscript_node(Node *base, Node *subscript)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NODE_SUBSCRIPT;
    node->data.subscript.base = base;
    node->data.subscript.sub = subscript;
    node->width = 0;
    node->height = 0;
    return node;
}

void free_ast(Node *node)
{
    if (!node)
        return;
    switch (node->type)
    {
    case NODE_SEQUENCE:
        free_ast(node->data.sequence.left);
        free_ast(node->data.sequence.right);
        break;
    case NODE_FRAC:
        free_ast(node->data.fraction.numerator);
        free_ast(node->data.fraction.denominator);
        break;
    case NODE_SUPERSCRIPT:
        free_ast(node->data.superscript.base);
        free_ast(node->data.superscript.sup);
        break;
    case NODE_SUBSCRIPT:
        free_ast(node->data.subscript.base);
        free_ast(node->data.subscript.sub);
        break;
    case NODE_CHARACTER:
        
        break;
    }
    free(node);
}

void calculate_dimensions(Node *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case NODE_CHARACTER:
        break;
    case NODE_SEQUENCE:
        calculate_dimensions(node->data.sequence.left);
        calculate_dimensions(node->data.sequence.right);
        node->width = node->data.sequence.left->width + CHAR_SPACING + node->data.sequence.right->width;
        node->height = (node->data.sequence.left->height >
                        node->data.sequence.right->height)
                           ? node->data.sequence.left->height
                           : node->data.sequence.right->height;
        break;

    case NODE_FRAC:
        calculate_dimensions(node->data.fraction.numerator);
        calculate_dimensions(node->data.fraction.denominator);
        node->width = (node->data.fraction.numerator->width > node->data.fraction.denominator->width ? node->data.fraction.numerator->width : node->data.fraction.denominator->width) + 2 * PADDING;
        node->height = node->data.fraction.numerator->height + node->data.fraction.denominator->height;
        break;
    case NODE_SUPERSCRIPT:
        calculate_dimensions(node->data.superscript.base);
        calculate_dimensions(node->data.superscript.sup);
        node->width = node->data.superscript.base->width + node->data.superscript.sup->width * SCRIPT_SCALE;
        node->height = node->data.superscript.base->height + node->data.superscript.sup->height * SCRIPT_SCALE;
        break;
    case NODE_SUBSCRIPT:
        calculate_dimensions(node->data.subscript.base);
        calculate_dimensions(node->data.subscript.sub);
        node->width = node->data.subscript.base->width + node->data.subscript.sub->width * SCRIPT_SCALE;
        node->height = node->data.subscript.base->height + node->data.subscript.sub->height * SCRIPT_SCALE;
        break;
    }
}

static char* render_char_centered(char ch, int cx, int cy, float scale)
{
    char* result = NULL;
    const SvgLetter *g = get_svg_letter(ch);

    if (g) {
        float tx = cx - (g->width  * scale) / 2.0f;
        float ty = cy + (g->height * scale) / 2.0f - 1 * scale;
        float g_scale_x = (g->width * scale / 600);
        float g_scale_y = -(g->height * scale / 600);

        result = string_append(result,
            "  <g transform=\"translate(%f,%f) scale(%f,%f)\">\n"
            "    <path d=\"%s\" fill=\"black\" stroke=\"black\" stroke-width=\"1\" />\n"
            "  </g>\n",
            tx, ty, g_scale_x, g_scale_y, g->svg_path
        );
    } else {
        char buf[2] = { ch, 0 };
        float fs = FONT_SIZE * scale;
        result = string_append(result,
          "  <text x=\"%d\" y=\"%d\" font-family=\"Arial\" font-size=\"%f\" "
          "fill=\"black\" text-anchor=\"middle\" dominant-baseline=\"middle\">%s</text>\n",
          cx, cy, fs, buf
        );
    }
    return result;
}


static void flatten(Node *n, char *out, size_t cap)
{
    if (!n || cap == 0)
        return;
    switch (n->type)
    {
    case NODE_CHARACTER:
        if (cap > 1)
        {
            *out++ = n->data.character.ch;
            *out = 0;
        }
        break;
    case NODE_SEQUENCE:
        flatten(n->data.sequence.left, out, cap);
        size_t used = strlen(out);
        flatten(n->data.sequence.right, out + used, cap - used);
        break;
    default:
        break;
    }
}

char* generate_svg(Node *node, int x, int y, float scale)
{
    char* result = NULL;
    if (!node)
        return result;

    switch (node->type)
    {
        case NODE_CHARACTER:
            result = render_char_centered(node->data.character.ch, x, y, scale);
            break;
        case NODE_SEQUENCE:
        {
            int start = x - node->width*scale / 2;
            if (start < 0) start = 0;
            start += node->data.sequence.left->width * scale / 2;
            char* left_svg = generate_svg(node->data.sequence.left, start, y, scale);

            int adv = node->data.sequence.left->width * scale / 2 + CHAR_SPACING * scale 
                      + node->data.sequence.right->width * scale / 2;
            char* right_svg = generate_svg(node->data.sequence.right, start + adv, y, scale);
            
            if (left_svg) {
                result = string_append(result, "%s", left_svg);
                free(left_svg);
            }
            if (right_svg) {
                result = string_append(result, "%s", right_svg);
                free(right_svg);
            }
            break;
        }

        case NODE_FRAC:
        {
            float sub = scale * FRAC_SCALE;

            int fracW = (int)((node->width - 2*PADDING) * scale);
            int numW = (int)(node->data.fraction.numerator->width * sub);
            int denW = (int)(node->data.fraction.denominator->width * sub);

            int barY = y;

            result = string_append(result, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                "stroke=\"black\" stroke-width=\"1\" />\n",
                x - fracW / 2, barY, x + fracW / 2, barY);

            int numX = x;
            int numY = barY - node->data.fraction.numerator->height * sub / 2 - FRAC_PADDING * sub;
            char* num_svg = generate_svg(node->data.fraction.numerator, numX, numY, sub);
            
            int denX = x;
            int denY = barY + node->data.fraction.denominator->height * sub / 2 + FRAC_PADDING * sub;
            char* den_svg = generate_svg(node->data.fraction.denominator, denX, denY, sub);
            
            if (num_svg) {
                result = string_append(result, "%s", num_svg);
                free(num_svg);
            }
            if (den_svg) {
                result = string_append(result, "%s", den_svg);
                free(den_svg);
            }
            break;
        }
        case NODE_SUPERSCRIPT:
        {
            float sub = scale * SCRIPT_SCALE;

            int baseX = x;
            int baseY = y;
            char* base_svg = generate_svg(node->data.superscript.base, baseX, baseY, scale);

            int supX = x + node->data.superscript.base->width / 2 + 5*scale;
            int supY = baseY - node->data.superscript.base->height / 2;
            char* sup_svg = generate_svg(node->data.superscript.sup, supX, supY, sub);
            
            if (base_svg) {
                result = string_append(result, "%s", base_svg);
                free(base_svg);
            }
            if (sup_svg) {
                result = string_append(result, "%s", sup_svg);
                free(sup_svg);
            }
            break;
        }
        case NODE_SUBSCRIPT:
        {
            float sub = scale * SCRIPT_SCALE;

            int baseX = x;
            int baseY = y;
            char* base_svg = generate_svg(node->data.subscript.base, baseX, baseY, scale);

            int subX = x + node->data.subscript.base->width / 2 + 5*scale;
            int subY = baseY + node->data.superscript.base->height / 2;
            char* sub_svg = generate_svg(node->data.subscript.sub, subX, subY, sub);
            
            if (base_svg) {
                result = string_append(result, "%s", base_svg);
                free(base_svg);
            }
            if (sub_svg) {
                result = string_append(result, "%s", sub_svg);
                free(sub_svg);
            }
            break;
        }
    }
    return result;
}

void print_ast(Node* node, int depth) {
    if (!node) return;
    
    
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    switch (node->type) {
        case NODE_CHARACTER:
            printf("CHARACTER: '%c' (w:%d, h:%d)\n", 
                   node->data.character.ch, node->width, node->height);
            break;
            
        case NODE_SEQUENCE:
            printf("SEQUENCE (w:%d, h:%d)\n", node->width, node->height);
            print_ast(node->data.sequence.left, depth + 1);
            print_ast(node->data.sequence.right, depth + 1);
            break;
            
        case NODE_FRAC:
            printf("FRACTION (w:%d, h:%d)\n", node->width, node->height);
            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("Numerator:\n");
            print_ast(node->data.fraction.numerator, depth + 2);
            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("Denominator:\n");
            print_ast(node->data.fraction.denominator, depth + 2);
            break;
            
        case NODE_SUPERSCRIPT:
            printf("SUPERSCRIPT (w:%d, h:%d)\n", node->width, node->height);
            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("Base:\n");
            print_ast(node->data.superscript.base, depth + 2);
            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("Superscript:\n");
            print_ast(node->data.superscript.sup, depth + 2);
            break;
            
        case NODE_SUBSCRIPT:
            printf("SUBSCRIPT (w:%d, h:%d)\n", node->width, node->height);
            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("Base:\n");
            print_ast(node->data.subscript.base, depth + 2);
            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("Subscript:\n");
            print_ast(node->data.subscript.sub, depth + 2);
            break;
    }
}

char* latex_to_svg_string(const char* input) {
    extern int yyparse();
    extern int yy_scan_string(const char*);
    extern Node* result_ast;
    
    if (!input) return NULL;
    
    yy_scan_string(input);
    if (yyparse() == 0 && result_ast) {
        calculate_dimensions(result_ast);
        
        int svg_width = result_ast->width + 40;
        
        char* output = NULL;
        
        output = string_append(output, "<svg width=\"%d\" height=\"150\">\n", svg_width);
        output = string_append(output, 
            "<defs>\n"
            "    <style type=\"text/css\"><![CDATA[\n"
            "    @font-face{\n"
            "        font-family:\"LM Roman\";\n"
            "        src:url(\"LMRoman10-Regular.otf\") format(\"opentype\");\n"
            "    }\n"
            "    text{ font-family:\"LM Roman\", serif; }\n"
            "    ]]></style>\n"
            "</defs>\n");

        char* svg_content = generate_svg(result_ast, 20, 75, 1.0);
        if (svg_content) {
            output = string_append(output, "%s", svg_content);
            free(svg_content);
        }
        
        output = string_append(output, "</svg>\n");
        
        free_ast(result_ast);
        result_ast = NULL;
        
        return output;
    }
    
    return NULL;
}