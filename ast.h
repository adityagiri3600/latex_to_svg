#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER,
    NODE_VARIABLE,
    NODE_BINARY_OP,
    NODE_FRAC
} NodeType;

struct Node;

typedef struct {
    int value;
} NumberNode;

typedef struct {
    char* name;
} VariableNode;

typedef struct {
    char op;
    struct Node* left;
    struct Node* right;
} BinaryOpNode;

typedef struct {
    struct Node* numerator;
    struct Node* denominator;
} FractionNode;

typedef struct Node {
    NodeType type;
    int width;
    union {
        NumberNode number;
        VariableNode variable;
        BinaryOpNode binary_op;
        FractionNode fraction;
    } data;
} Node;


Node* create_number_node(int value);
Node* create_variable_node(char* name);
Node* create_binary_op_node(char op, Node* left, Node* right);
Node* create_fraction_node(Node* numerator, Node* denominator);
void free_ast(Node* node);

void calculate_widths(Node* node);
void generate_svg(Node* node, int x, int y);

#endif
