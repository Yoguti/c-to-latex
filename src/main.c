// Manages the input CSV and the compilation pipeline (Lex -> Parse -> Emit)
#include "errors.h"
#include "lexer.h"
#include "parser.h"
#include "emitter.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Program Usage: ./program path/to/my/file.c");
        panic(ERR_WRONG_ARG_NUM,0);
    } else {
        const char *filepath = argv[1];
        // Lexing
        TokenList *list = lex_file(filepath);
        if (!list) {
            panic(ERR_FILE_NOT_FOUND, 0);
        }
        printList(list);
        // Parsing
        // ASTNode *ast = parse_tokens(list);
        // if (!ast) {
        //     panic(ERR_UNEXPECTED_TOKEN);
        // }
        // Emitting
        //emit(ast);
    }
}