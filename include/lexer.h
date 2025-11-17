// Scans the raw input string character-by-character and converts it into a flat list of classified tokens
#ifndef LEXER_H
#define LEXER_H

// Doesn't include TOKENS for commments or preprocessor 
// hooks, as they are not allowed into the file

typedef enum {
    TOKEN_EOF = 1,            // End of token stream
    TOKEN_UNKNOWN = 2,        // Unknown value 

    // Literals
    TOKEN_INT_LITERAL = 3,    // 123
    TOKEN_FLOAT_LITERAL = 4,  // 123.45, .5, 10.
    TOKEN_CHAR_LITERAL = 5,   // 'a'
    TOKEN_STRING_LITERAL=6, // "Hello World"

    // Identifiers & Keywords
    TOKEN_PREPROCESSOR=7,
    TOKEN_IDENTIFIER=8,     // main, x, myVar
    TOKEN_KEYWORD=9,        // return, int, if, while

    // Operators & Punctuation
    TOKEN_ASSIGNMENT_OPERATOR=10, // += -= *=
    TOKEN_OPERATOR=11,       // +, -, =, ==, *
    TOKEN_DOT=12,            // .
    TOKEN_COMMA=13,          // ,
    TOKEN_SEMICOLON=14,      // ;
    TOKEN_BITWISE_OPERATOR=15, // & , | , <<, >>, ^, ~, `
    TOKEN_LOGIC_OPERATOR=16, // &&, ||, !
    
    // Grouping
    TOKEN_COMMENT=17, // // or /**/
    TOKEN_PAREN_OPEN=18,     // (
    TOKEN_PAREN_CLOSE=19,    // )
    TOKEN_BRACE_OPEN=20,     // {
    TOKEN_BRACE_CLOSE=21,    // }
    TOKEN_BRACKET_OPEN=22,   // [
    TOKEN_BRACKET_CLOSE=23,   // ]
    TOKEN_ARROW=24,           // ->

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

void printList(TokenList *list);
const char* printEnum(unsigned int enumber);
void add_token(TokenList *list, Token *token);
TokenList* lex_file(const char *filepath);
void free_token_list(TokenList *list);
int free_token(Token *token);
TokenList* create_token_list();
Token* create_token(TokenType type, const char* value);
TokenType check_keyword(const char* text);

#endif // LEXER_H