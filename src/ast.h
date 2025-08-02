#ifndef AST_H
#define AST_H

typedef enum {
    NODE_CHARACTER,
    NODE_SEQUENCE,
    NODE_FRAC
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

typedef struct Node {
    NodeType type;
    int width;
    int height;
    union {
        CharacterNode character;
        SequenceNode sequence;
        FractionNode fraction;
    } data;
} Node;


Node* create_character_node(char ch);
Node* create_sequence_node(Node* left, Node* right);
Node* create_fraction_node(Node* numerator, Node* denominator);
void free_ast(Node* node);

void calculate_dimensions(Node* node);
void generate_svg(Node* node, int x, int y, float scale);

#endif
