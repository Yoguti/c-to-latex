#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

const char* get_error_message(ErrorCode code) {
    switch (code) {
        case ERR_MAX_SIZE: return "Reached maximum size limit while parsing a token";
        case ERR_MALFORMED_FLOAT: return "Malformed float value!";
        case ERR_FREE_MEMORY: return "Could not free memory, halting execution";
        case ERR_WRONG_ARG_NUM: return "Invalid number of program arguments! Remember to include the filepath";
        case ERR_UNEXPECTED_CHAR: return "Unexpected character encountered";
        case ERR_UNEXPECTED_TOKEN: return "Unexpected token in syntax";
        case ERR_MISSING_PAREN: return "Missing parenthesis";
        case ERR_UNKNOWN_FUNCTION: return "Unknown function";
        case ERR_MEMORY_ALLOCATION: return "Memory allocation failed";
        case ERR_FILE_NOT_FOUND: return "File not found";
        default: return "Unknown error";
    }
}

void panic(ErrorCode code, int current_line) {
    if (current_line > 0)
    {
        fprintf(stderr, "Error: %s in line %d\n", get_error_message(code), current_line);
    }
    
    fprintf(stderr, "Error: %s\n", get_error_message(code));
    exit(EXIT_FAILURE);
}

