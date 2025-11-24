// Defines the Abstract Syntax Tree data structures (nodes, enums) and manages memory allocation
// ast.h

typedef enum {
    AST_PROGRAM,        // The root node (holds list of functions/globals)
    AST_FUNCTION_DECL,  // int main() { ... }
    AST_VAR_DECL,       // int x = 5;
    AST_BINARY_OP,      // 5 + 3
    AST_INT_LITERAL     // 5
} NodeType;

// Forward declaration so we can use it inside the struct
struct ASTNode;

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
            struct ASTNode *body; // This points to a block of code
        } function;

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