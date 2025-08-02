#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


Node* create_number_node(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_NUMBER;
    node->data.number.value = value;
    return node;
}

Node* create_variable_node(char* name) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_VARIABLE;
    node->data.variable.name = strdup(name);
    return node;
}

Node* create_binary_op_node(char op, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_BINARY_OP;
    node->data.binary_op.op = op;
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    return node;
}

Node* create_fraction_node(Node* numerator, Node* denominator) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NODE_FRAC;
    node->data.fraction.numerator = numerator;
    node->data.fraction.denominator = denominator;
    return node;
}

void free_ast(Node* node) {
    if (!node) return;
    switch (node->type) {
        case NODE_VARIABLE:
            free(node->data.variable.name);
            break;
        case NODE_BINARY_OP:
            free_ast(node->data.binary_op.left);
            free_ast(node->data.binary_op.right);
            break;
        case NODE_FRAC:
            free_ast(node->data.fraction.numerator);
            free_ast(node->data.fraction.denominator);
            break;
        default:
            break;
    }
    free(node);
}


const int CIRCLE_RADIUS = 20;
const int PADDING = 10;

void calculate_widths(Node* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_NUMBER:
        case NODE_VARIABLE:
            node->width = 2 * CIRCLE_RADIUS;
            break;
        case NODE_BINARY_OP:
            calculate_widths(node->data.binary_op.left);
            calculate_widths(node->data.binary_op.right);
            node->width = node->data.binary_op.left->width + 
                          node->data.binary_op.right->width + 
                          2 * CIRCLE_RADIUS + 2 * PADDING;
            break;
        case NODE_FRAC:
            calculate_widths(node->data.fraction.numerator);
            calculate_widths(node->data.fraction.denominator);
            node->width = (node->data.fraction.numerator->width > node->data.fraction.denominator->width ? 
                           node->data.fraction.numerator->width : node->data.fraction.denominator->width) + 2 * PADDING;
            break;
    }
}

void generate_svg(Node* node, int x, int y) {
    if (!node) return;

    switch (node->type) {
        case NODE_NUMBER:
        case NODE_VARIABLE:
            printf("  <circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"blue\" stroke=\"black\" stroke-width=\"2\"/>\n", x + CIRCLE_RADIUS, y, CIRCLE_RADIUS);
            break;
        case NODE_BINARY_OP:
            generate_svg(node->data.binary_op.left, x, y);
            printf("  <circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"red\" stroke=\"black\" stroke-width=\"2\"/>\n", 
                   x + node->data.binary_op.left->width + PADDING + CIRCLE_RADIUS, y, CIRCLE_RADIUS);
            generate_svg(node->data.binary_op.right, x + node->data.binary_op.left->width + 2 * PADDING + 2 * CIRCLE_RADIUS, y);
            break;
        case NODE_FRAC:

            printf("  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" stroke-width=\"2\"/>\n",
                   x, y, x + node->width, y);
            generate_svg(node->data.fraction.numerator, x + (node->width - node->data.fraction.numerator->width) / 2, y - 30);
            generate_svg(node->data.fraction.denominator, x + (node->width - node->data.fraction.denominator->width) / 2, y + 30);
            break;
    }
}
