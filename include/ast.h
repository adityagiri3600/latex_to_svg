#ifndef AST_H
#define AST_H

typedef enum {
    NODE_CHARACTER,
    NODE_SEQUENCE,
    NODE_FRAC,
    NODE_SUPERSCRIPT,
    NODE_SUBSCRIPT
} NodeType;

struct Node;

typedef struct {
    char ch;
} CharacterNode;

typedef struct {
    struct Node* left;
    struct Node* right;
} SequenceNode;

typedef struct {
    struct Node* numerator;
    struct Node* denominator;
} FractionNode;

typedef struct {
    struct Node* base;
    struct Node* sup;
} SuperscriptNode;

typedef struct {
    struct Node* base;
    struct Node* sub;
} SubscriptNode;

typedef struct Node {
    NodeType type;
    int width;
    int height;
    union {
        CharacterNode character;
        SequenceNode sequence;
        FractionNode fraction;
        SuperscriptNode superscript;
        SubscriptNode subscript;
    } data;
} Node;


Node* create_character_node(char ch);
Node* create_sequence_node(Node* left, Node* right);
Node* create_fraction_node(Node* numerator, Node* denominator);
Node* create_superscript_node(Node* base, Node* exponent);
Node* create_subscript_node(Node* base, Node* subscript);
void free_ast(Node* node);

void calculate_dimensions(Node* node);
char* generate_svg(Node* node, int x, int y, float scale);
void print_ast(Node* node, int depth);

#endif
