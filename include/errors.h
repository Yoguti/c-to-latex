// Error definitions and reporting functions to handle crashes or invalid syntax.

#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    ERR_MAX_SIZE,
    ERR_MALFORMED_FLOAT,
    ERR_SYNTAX_ERROR,
    ERR_FREE_MEMORY,
    ERR_WRONG_ARG_NUM,
    ERR_UNEXPECTED_CHAR,
    ERR_UNEXPECTED_TOKEN,
    ERR_MISSING_PAREN,
    ERR_UNKNOWN_FUNCTION,
    ERR_MEMORY_ALLOCATION,
    ERR_FILE_NOT_FOUND
} ErrorCode;

const char* get_error_message(ErrorCode error);
void panic(ErrorCode error, int current_line);
#endif // ERRORS_H