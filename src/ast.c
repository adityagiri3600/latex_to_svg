#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

const int CHAR_WIDTH = 12;
const int CHAR_HEIGHT = 20;
const int PADDING = 10;
const int FRAC_PADDING = 5; // Space above and below the fraction bar
const int CHAR_SPACING = 5; // Space between characters in a sequence
const float FRAC_SCALE = 0.9f;  // 70% scale for numerator/denominator

Node* create_character_node(char ch) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_CHARACTER;
    node->data.character.ch = ch;
    node->width = CHAR_WIDTH;
    node->height = CHAR_HEIGHT;
    return node;
}

Node* create_sequence_node(Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_SEQUENCE;
    node->data.sequence.left = left;
    node->data.sequence.right = right;
    node->width = 0;
    return node;
}

Node* create_fraction_node(Node* numerator, Node* denominator) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_FRAC;
    node->data.fraction.numerator = numerator;
    node->data.fraction.denominator = denominator;
    node->width = 0;
    return node;
}

void free_ast(Node* node) {
    if (!node) return;
    switch (node->type) {
        case NODE_SEQUENCE:
            free_ast(node->data.sequence.left);
            free_ast(node->data.sequence.right);
            break;
        case NODE_FRAC:
            free_ast(node->data.fraction.numerator);
            free_ast(node->data.fraction.denominator);
            break;
        case NODE_CHARACTER:
            // No additional memory to free
            break;
    }
    free(node);
}

void calculate_dimensions(Node* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_CHARACTER:
            break;
        case NODE_SEQUENCE:
            calculate_dimensions(node->data.sequence.left);
            calculate_dimensions(node->data.sequence.right);
            node->width = node->data.sequence.left->width + node->data.sequence.right->width;
            node->height = node->data.sequence.left->height > node->data.sequence.right->height ? 
                           node->data.sequence.left->height : node->data.sequence.right->height;
            break;
        case NODE_FRAC:
            calculate_dimensions(node->data.fraction.numerator);
            calculate_dimensions(node->data.fraction.denominator);
            node->width = (node->data.fraction.numerator->width > node->data.fraction.denominator->width ? 
                           node->data.fraction.numerator->width : node->data.fraction.denominator->width) + 2 * PADDING;
            node->height = node->data.fraction.numerator->height + node->data.fraction.denominator->height + PADDING;
            break;
    }
}

static void render_text_centered(const char* s, int cx, int cy, float scale) {
    float fs = 20 * scale; /* only use font-size for scaling */
    printf(
      "  <text x=\"%d\" y=\"%d\" font-family=\"Arial\" font-size=\"%f\" "
      "fill=\"black\" text-anchor=\"middle\" dominant-baseline=\"middle\">%s</text>\n",
      cx, cy, fs, s
    );
}

static void flatten(Node* n, char* out, size_t cap) {
    if (!n || cap == 0) return;
    switch (n->type) {
        case NODE_CHARACTER:
            if (cap > 1) { *out++ = n->data.character.ch; *out = 0; }
            break;
        case NODE_SEQUENCE:
            flatten(n->data.sequence.left, out, cap);
            size_t used = strlen(out);
            flatten(n->data.sequence.right, out + used, cap - used);
            break;
        default: /* fractions are handled separately */ break;
    }
}

void generate_svg(Node* node, int x, int y, float scale) {
    if (!node) return;

    switch (node->type) {
        case NODE_CHARACTER: {
            char buf[2] = { node->data.character.ch, 0 };
            render_text_centered(buf, x, y, scale);
            break;
        }
        case NODE_SEQUENCE: {
            generate_svg(node->data.sequence.left,  x, y, scale);

            int leftW = (int)(node->data.sequence.left->width * scale);
            generate_svg(node->data.sequence.right, x + leftW, y, scale);
            break;
        }
        case NODE_FRAC: {
            float sub = scale * FRAC_SCALE;

            int numW  = (int)(node->data.fraction.numerator  ->width * sub);
            int denW  = (int)(node->data.fraction.denominator->width * sub);

            int fracW = node->width - 2 * PADDING;

            int barY = y;
            printf("  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                "stroke=\"black\" stroke-width=\"2\" />\n",
                x, barY, x + fracW, barY);

            int numX = x + (fracW - numW) / 2;
            int numY = barY - node->data.fraction.numerator->height * sub / 2 - FRAC_PADDING * scale;
            printf("Height of numerator: %d\n", node->data.fraction.numerator->height);
            generate_svg(node->data.fraction.numerator, numX, numY, sub);
            
            int denX = x + (fracW - denW) / 2;
            int denY = barY + node->data.fraction.denominator->height * sub / 2 + FRAC_PADDING * scale;
            printf("Height of denominator: %d\n", node->data.fraction.denominator->height);
            generate_svg(node->data.fraction.denominator, denX, denY, sub);
            break;
        }


    }
}
