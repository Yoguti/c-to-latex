// Scans the raw input string character-by-character and converts it into a flat list of classified tokens
#ifndef LEXER_H
#define LEXER_H

// Doesn't include TOKENS for commments or preprocessor 
// hooks, as they are not allowed into the file

typedef enum {
    TOKEN_EOF,            // End of token stream
    TOKEN_UNKNOWN,        // Unknown value 

    // Literals
    TOKEN_INT_LITERAL,    // 123
    TOKEN_FLOAT_LITERAL,  // 123.45, .5, 10.
    TOKEN_CHAR_LITERAL,   // 'a'
    TOKEN_STRING_LITERAL, // "Hello World"

    // Identifiers & Keywords
    TOKEN_PREPROCESSOR,
    TOKEN_IDENTIFIER,     // main, x, myVar
    TOKEN_KEYWORD,        // return, int, if, while

    // Operators & Punctuation
    TOKEN_ASSIGNMENT_OPERATOR, // += -= *=
    TOKEN_OPERATOR,       // +, -, =, ==, *
    TOKEN_DOT,            // .
    TOKEN_COMMA,          // ,
    TOKEN_SEMICOLON,      // ;
    TOKEN_BITWISE_OPERATOR, // & , | , <<, >>, ^, ~, `
    TOKEN_LOGIC_OPERATOR, // &&, ||, !
    
    // Grouping
    TOKEN_COMMENT, // // or /**/
    TOKEN_PAREN_OPEN,     // (
    TOKEN_PAREN_CLOSE,    // )
    TOKEN_BRACE_OPEN,     // {
    TOKEN_BRACE_CLOSE,    // }
    TOKEN_BRACKET_OPEN,   // [
    TOKEN_BRACKET_CLOSE,  // ]

} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct TokenList {
    Token **tokens;
    int count;
    int capacity;
} TokenList;

void add_token(TokenList *list, Token *token);
TokenList* lex_file(const char *filepath);
void free_token_list(TokenList *list);
int free_token(Token *token);
TokenList* create_token_list();
Token* create_token(TokenType type, const char* value);
TokenType check_keyword(const char* text);

#endif // LEXER_H