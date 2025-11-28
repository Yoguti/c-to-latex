// Defines the Abstract Syntax Tree data structures (nodes, enums) and manages memory allocation
// ast.h
#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,        // root node (holds list of functions/globals)
    AST_FUNCTION_DECL,  // int main()
    AST_BLOCK, // represents {}
    AST_VAR_DECL,       // int x = 5;
    AST_BINARY_OP,      // 5 + 3
    AST_INT_LITERAL     // 5
} NodeType;

struct ASTNode;

/*
This implementation has a major memory consumption trade-off by using
a union inside the ASTNode, we could save memory by using a "base node" approach where each node type is its own struct
inheriting from a base ASTNode struct, but that would make things more complex to manage.
*/

typedef struct ASTNode {
    NodeType type;
    union {
        // Data for AST_PROGRAM
        struct {
            struct ASTNode **statements; // Array of pointers (functions/globals)
            int statement_count;
        } program;

        // Data for AST_FUNCTION_DECL
        struct {
            char *name;
            char *return_type;
            struct ASTNode *body; // points to a block of code
        } function;

        // Data for AST_BLOCK
        struct {
            struct ASTNode **statements; // compound statements
            int statement_count;
        } block;

        // Data for AST_BINARY_OP
        struct {
            char *operator;       // "+", "-", "=="
            struct ASTNode *left;
            struct ASTNode *right;
        } bin_op;

        // Data for AST_INT_LITERAL
        int int_val; 
    } data;
} ASTNode;

#endif
