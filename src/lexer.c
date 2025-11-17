#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "errors.h"

const int MAX_TOKEN_VALUE_LENGTH = 255;
const int MAX_CHAR_VALUE = 10;

void printList(TokenList *list) {
    for (int i = 0; i < list->count; i++)
    {
        Token *tok = list->tokens[i];
        printf("%s : %s\n", printEnum(tok->type), tok->value);
    }
    
}

const char* printEnum(unsigned int enumber) {
    switch (enumber) {
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_UNKNOWN: return "TOKEN_UNKNOWN";
        case TOKEN_INT_LITERAL: return "TOKEN_INT_LITERAL";
        case TOKEN_FLOAT_LITERAL: return "TOKEN_FLOAT_LITERAL";
        case TOKEN_CHAR_LITERAL: return "TOKEN_CHAR_LITERAL";
        case TOKEN_STRING_LITERAL: return "TOKEN_STRING_LITERAL";
        case TOKEN_PREPROCESSOR: return "TOKEN_PREPROCESSOR";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TOKEN_ASSIGNMENT_OPERATOR: return "TOKEN_ASSIGNMENT_OPERATOR";
        case TOKEN_OPERATOR: return "TOKEN_OPERATOR";
        case TOKEN_DOT: return "TOKEN_DOT";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_BITWISE_OPERATOR: return "TOKEN_BITWISE_OPERATOR";
        case TOKEN_LOGIC_OPERATOR: return "TOKEN_LOGIC_OPERATOR";
        case TOKEN_COMMENT: return "TOKEN_COMMENT";
        case TOKEN_PAREN_OPEN: return "TOKEN_PAREN_OPEN";
        case TOKEN_PAREN_CLOSE: return "TOKEN_PAREN_CLOSE";
        case TOKEN_BRACE_OPEN: return "TOKEN_BRACE_OPEN";
        case TOKEN_BRACE_CLOSE: return "TOKEN_BRACE_CLOSE";
        case TOKEN_BRACKET_OPEN: return "TOKEN_BRACKET_OPEN";
        case TOKEN_BRACKET_CLOSE: return "TOKEN_BRACKET_CLOSE";
        case TOKEN_ARROW: return "TOKEN_ARROW";
        default: return "TOKEN_INVALID";
    }
}

TokenList *create_token_list()
{
    TokenList *list = (TokenList *)calloc(1, sizeof(TokenList));
    if (!list)
    {
        panic(ERR_MEMORY_ALLOCATION, 0);
        return NULL;
    }
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 10;
    list->tokens = (Token **)calloc(list->capacity, sizeof(Token *));
    if (!list->tokens)
    {
        panic(ERR_MEMORY_ALLOCATION, 0);
        free(list);
        return NULL;
    }
    return list;
}

void add_token(TokenList *list, Token *token)
{
    if (list->count >= list->capacity)
    {
        list->capacity *= 2;
        Token **new_tokens = (Token **)realloc(list->tokens, sizeof(Token *) * list->capacity);
        if (!new_tokens)
        {
            panic(ERR_MEMORY_ALLOCATION, 0);
        }
        list->tokens = new_tokens;
    }
    list->tokens[list->count++] = token;
}

void free_token_list(TokenList *list)
{
    if (list == NULL)
        return;
    for (int i = 0; i < list->count; i++)
    {
        if (list->tokens[i] == NULL)
            continue;
        if (free_token(list->tokens[i]))
        {
            panic(ERR_FREE_MEMORY, 0);
        }
    }
    free(list->tokens);
    free(list);
}

int free_token(Token *token)
{
    if (!token)
        return 1;
    free(token->value);
    free(token);
    return 0;
}

Token *create_token(TokenType type, const char *value)
{
    Token *new_token = (Token *)malloc(sizeof(Token));
    if (!new_token)
    {
        panic(ERR_MEMORY_ALLOCATION, 0);
        return NULL;
    }

    new_token->type = type;
    new_token->value = strdup(value);

    if (!new_token->value)
    {
        free(new_token);
        panic(ERR_MEMORY_ALLOCATION, 0);
        return NULL;
    }
    return new_token;
}

TokenType check_keyword(const char *text)
{
    static const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

    for (int i = 0; i < 32; i++)
    {
        if (strcmp(text, keywords[i]) == 0)
        {
            return TOKEN_KEYWORD;
        }
    }
    return TOKEN_IDENTIFIER;
}

TokenList *lex_file(const char *filepath)
{
    int current_line = 1;

    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        panic(ERR_FILE_NOT_FOUND, 0);
        return NULL;
    }

    TokenList *tokenList = create_token_list();

    int ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            current_line++;
            continue;
        }
        if (isspace(ch))
            continue;
        // handle == comparison
        if (ch == '=')
        {
            int next_ch = fgetc(file);
            if (next_ch == '=')
            {
                add_token(tokenList, create_token(TOKEN_OPERATOR, "=="));
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_OPERATOR, "="));
            }
            continue;
        }
        // handle bitwise operators
        if (ch == '<')
        {
            int next_ch = fgetc(file);
            if (next_ch == '<')
            {
                add_token(tokenList, create_token(TOKEN_BITWISE_OPERATOR, "<<"));
            }
            else if (next_ch == '=')
            {
                add_token(tokenList, create_token(TOKEN_OPERATOR, "<="));
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_BITWISE_OPERATOR, "<"));
            }
            continue;
        }
        if (ch == '>')
        {
            int next_ch = fgetc(file);
            if (next_ch == '>')
            {
                add_token(tokenList, create_token(TOKEN_BITWISE_OPERATOR, ">>"));
            }
            else if (next_ch == '=')
            {
                add_token(tokenList, create_token(TOKEN_OPERATOR, ">="));
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_BITWISE_OPERATOR, ">"));
            }
            continue;
        }
        // handle != comparison
        if (ch == '!')
        {
            int next_ch = fgetc(file);
            if (next_ch == '=')
            {
                add_token(tokenList, create_token(TOKEN_OPERATOR, "!="));
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_LOGIC_OPERATOR, "!"));
            }
            continue;
        }
        // handle += -= *= ++ --
        if (ch == '+')
        {
            int next_ch = fgetc(file);
            if (next_ch == '=')
            {
                add_token(tokenList, create_token(TOKEN_ASSIGNMENT_OPERATOR, "+="));
            }
            else if (next_ch == '+')
            {
                add_token(tokenList, create_token(TOKEN_ASSIGNMENT_OPERATOR, "++"));
            }
            else
            {
                add_token(tokenList, create_token(TOKEN_ASSIGNMENT_OPERATOR, "+"));
            }
            continue;
        }
        if (ch == '-')
        {
            int next_ch = fgetc(file);
            if (next_ch == '=')
            {
                add_token(tokenList, create_token(TOKEN_ASSIGNMENT_OPERATOR, "-="));
            }
            else if (next_ch == '-')
            {
                add_token(tokenList, create_token(TOKEN_ASSIGNMENT_OPERATOR, "--"));
            }
            else if (next_ch == '>')
            {
                add_token(tokenList, create_token(TOKEN_ARROW, "->"));
            }
            else
            {
                add_token(tokenList, create_token(TOKEN_ASSIGNMENT_OPERATOR, "-"));
            }
            continue;
        }
        // handle pointers and *=
        if (ch == '*')
        {
            int next_ch = fgetc(file);
            if (next_ch == '=')
            {
                add_token(tokenList, create_token(TOKEN_ASSIGNMENT_OPERATOR, "*="));
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_OPERATOR, "*"));
            }
            continue;
        }
        // handle logical operators
        if (ch == '&')
        {
            int next_ch = fgetc(file);

            if (next_ch == '&')
            {
                add_token(tokenList, create_token(TOKEN_LOGIC_OPERATOR, "&&"));
            }
            else if (next_ch == '=')
            {
                add_token(tokenList, create_token(TOKEN_ASSIGNMENT_OPERATOR, "&="));
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_BITWISE_OPERATOR, "&"));
            }
            continue;
        }

        if (ch == '|')
        {
            int next_ch = fgetc(file);
            if (next_ch == '|')
            {
                add_token(tokenList, create_token(TOKEN_LOGIC_OPERATOR, "||"));
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_BITWISE_OPERATOR, "|"));
            }
            continue;
        }

        // handle division
        if (ch == '/')
        {
            int next_ch = fgetc(file);
            char buffer[MAX_TOKEN_VALUE_LENGTH + 1];
            if (next_ch == '=')
            {
                add_token(tokenList, create_token(TOKEN_OPERATOR, "/="));
            }
            else if (next_ch == '/')
            {
                int i = 0;
                while ((next_ch = fgetc(file)) != EOF && next_ch != '\n')
                {
                    if (i < MAX_TOKEN_VALUE_LENGTH - 1)
                    {
                        buffer[i++] = next_ch;
                    }
                    else
                    {
                        panic(ERR_MAX_SIZE, current_line);
                    }
                }

                buffer[i] = '\0'; // Null-terminate
                add_token(tokenList, create_token(TOKEN_COMMENT, buffer));
            }
            else if (next_ch == '*')
            {
                int prev_ch = 0;
                int i = 0;
                while ((next_ch = fgetc(file)) != EOF)
                {
                    if (prev_ch == '*' && next_ch == '/')
                    {
                        break;
                    }
                    if (next_ch == '\n')
                    {
                        current_line++;
                    }
                    prev_ch = next_ch;
                    if (i < MAX_TOKEN_VALUE_LENGTH)
                    {
                        int temp = fgetc(file);
                        if (next_ch == '*' && temp == '/')
                        {
                            ungetc(temp, file);
                            continue;
                        }
                        ungetc(temp, file);
                        buffer[i++] = next_ch;
                    }
                    else
                    {
                        panic(ERR_MAX_SIZE, current_line);
                    }
                }
                buffer[i] = '\0';
                add_token(tokenList, create_token(TOKEN_COMMENT, buffer));
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_OPERATOR, "/"));
            }
            continue;
        }

        // handle keyword
        if (isalpha(ch) || ch == '_')
        {
            char buffer[MAX_TOKEN_VALUE_LENGTH + 1];
            int i = 0;

            buffer[i++] = ch;

            while (isalnum(ch = fgetc(file)) || ch == '_')
            {
                if (i < MAX_TOKEN_VALUE_LENGTH)
                {
                    buffer[i++] = ch;
                }
                else
                {
                    panic(ERR_MAX_SIZE, current_line);
                }
            }

            buffer[i] = '\0';
            if (ch != EOF)
                ungetc(ch, file);

            TokenType type = check_keyword(buffer);
            add_token(tokenList, create_token(type, buffer));
            continue;
        }
        // handle floats
        if (ch == '.')
        {
            char buffer[256];
            int next_ch = fgetc(file);
            if (!isdigit(next_ch))
            {
                add_token(tokenList, create_token(TOKEN_DOT, "."));
                if (next_ch != EOF)
                    ungetc(next_ch, file);
            }
            else
            {
                int i = 0;
                buffer[i++] = ch;
                buffer[i++] = next_ch;
                next_ch = fgetc(file);
                while (isdigit(next_ch))
                {
                    if (i < MAX_TOKEN_VALUE_LENGTH)
                    {
                        buffer[i++] = next_ch;
                    }
                    else
                    {
                        panic(ERR_MAX_SIZE, current_line);
                    }
                    next_ch = fgetc(file);
                }
                if (next_ch == 'e' || next_ch == 'E')
                {
                    if (i < MAX_TOKEN_VALUE_LENGTH)
                    {
                        buffer[i++] = next_ch;
                    }
                    else
                    {
                        panic(ERR_MAX_SIZE, current_line);
                    }
                    next_ch = fgetc(file);
                    if (next_ch == '+' || next_ch == '-')
                    {
                        if (i < MAX_TOKEN_VALUE_LENGTH)
                        {
                            buffer[i++] = next_ch;
                        }
                        else
                        {
                            panic(ERR_MAX_SIZE, current_line);
                        }
                        next_ch = fgetc(file);
                    }
                    if (!isdigit(next_ch))
                        panic(ERR_MALFORMED_FLOAT, current_line);
                    while (isdigit(next_ch))
                    {
                        if (i < MAX_TOKEN_VALUE_LENGTH)
                        {
                            buffer[i++] = next_ch;
                        }
                        else
                        {
                            panic(ERR_MAX_SIZE, current_line);
                        }
                        next_ch = fgetc(file);
                    }
                }
                buffer[i] = '\0';
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_FLOAT_LITERAL, buffer));
            }
            continue;
        }

        if (isdigit(ch))
        {
            char buffer[256];
            int i = 0;
            int is_float = 0;

            buffer[i++] = ch;
            int next_ch = fgetc(file);

            while (isdigit(next_ch))
            {
                if (i < MAX_TOKEN_VALUE_LENGTH)
                {
                    buffer[i++] = next_ch;
                }
                else
                {
                    panic(ERR_MAX_SIZE, current_line);
                };
                next_ch = fgetc(file);
            }

            if (next_ch == '.')
            {
                is_float = 1;
                if (i < MAX_TOKEN_VALUE_LENGTH)
                {
                    buffer[i++] = next_ch;
                }
                else
                {
                    panic(ERR_MAX_SIZE, current_line);
                };
                next_ch = fgetc(file);
                while (isdigit(next_ch))
                {
                    if (i < MAX_TOKEN_VALUE_LENGTH)
                    {
                        buffer[i++] = next_ch;
                    }
                    else
                    {
                        panic(ERR_MAX_SIZE, current_line);
                    };
                    next_ch = fgetc(file);
                }
            }

            if (next_ch == 'e' || next_ch == 'E')
            {
                is_float = 1;
                if (i < MAX_TOKEN_VALUE_LENGTH)
                {
                    buffer[i++] = next_ch;
                }
                else
                {
                    panic(ERR_MAX_SIZE, current_line);
                };
                next_ch = fgetc(file);
                if (next_ch == '+' || next_ch == '-')
                {
                    if (i < MAX_TOKEN_VALUE_LENGTH)
                    {
                        buffer[i++] = next_ch;
                    }
                    else
                    {
                        panic(ERR_MAX_SIZE, current_line);
                    };
                    next_ch = fgetc(file);
                }
                if (!isdigit(next_ch))
                    panic(ERR_MALFORMED_FLOAT, current_line);
                while (isdigit(next_ch))
                {
                    if (i < MAX_TOKEN_VALUE_LENGTH)
                    {
                        buffer[i++] = next_ch;
                    }
                    else
                    {
                        panic(ERR_MAX_SIZE, current_line);
                    };
                    next_ch = fgetc(file);
                }
            }

            buffer[i] = '\0';
            if (next_ch != EOF)
                ungetc(next_ch, file);

            if (is_float)
            {
                add_token(tokenList, create_token(TOKEN_FLOAT_LITERAL, buffer));
            }
            else
            {
                add_token(tokenList, create_token(TOKEN_INT_LITERAL, buffer));
            }
            continue;
        }
        if (ch == '\'')
        {
            char buffer[MAX_CHAR_VALUE + 1];
            buffer[0] = '\0';
            int next_ch = fgetc(file);
            int i = 0;
            while (next_ch != EOF)
            {
                if (!(next_ch == '\''))
                {
                    if (i < MAX_CHAR_VALUE - 1)
                    {
                        buffer[i++] = next_ch;
                        next_ch = fgetc(file);
                    }
                    else
                    {
                        panic(ERR_SYNTAX_ERROR, current_line);
                    }
                }
                else
                {
                    buffer[i] = '\0';
                    add_token(tokenList, create_token(TOKEN_CHAR_LITERAL, buffer));
                    break;
                }
            }
            if (i == 0)
            {
                panic(ERR_SYNTAX_ERROR, current_line);
            }
            if (next_ch == EOF)
            { // avoid malformed char literal like 'a
                panic(ERR_SYNTAX_ERROR, current_line);
            }
            continue;
        }

        if (ch == '"')
        {
            char buffer[MAX_TOKEN_VALUE_LENGTH + 1];
            int i = 0;
            int current_ch;

            while ((current_ch = fgetc(file)) != EOF)
            {
                if (current_ch == '"')
                    break; // End of string

                if (current_ch == '\\')
                {
                    if (i < MAX_TOKEN_VALUE_LENGTH - 1)
                    {
                        buffer[i++] = current_ch;
                        current_ch = fgetc(file);
                        buffer[i++] = current_ch;
                    }
                    else
                    {
                        fgetc(file);
                        panic(ERR_MAX_SIZE, current_line);
                    }
                    continue;
                }

                if (i < MAX_TOKEN_VALUE_LENGTH - 1)
                {
                    buffer[i++] = current_ch;
                }
                else
                {
                    panic(ERR_MAX_SIZE, current_line);
                }
            }
            buffer[i] = '\0';
            add_token(tokenList, create_token(TOKEN_STRING_LITERAL, buffer));
            continue;
        }

        if (ch == '?')
        {
            int next_ch = fgetc(file);
            if (next_ch == ':')
            {
                add_token(tokenList, create_token(TOKEN_LOGIC_OPERATOR, "?:"));
            }
            else
            {
                if (next_ch != EOF)
                    ungetc(next_ch, file);
                add_token(tokenList, create_token(TOKEN_OPERATOR, "?"));
            }
            continue;
        }

        if (ch == '#')
        {
            char buffer[MAX_TOKEN_VALUE_LENGTH + 1];
            int i = 0;
            buffer[i++] = '#';

            int next_ch;
            while ((next_ch = fgetc(file)) != EOF && next_ch != '\n')
            {
                if (i < MAX_TOKEN_VALUE_LENGTH - 1)
                {
                    buffer[i++] = next_ch;
                }
                else
                {
                    panic(ERR_MAX_SIZE, current_line);
                }
            }
            buffer[i] = '\0';
            add_token(tokenList, create_token(TOKEN_PREPROCESSOR, buffer));
            continue;
        }

        // if it wasn't anything else, then it's simple punctuation or operator
        switch (ch)
        {
        case '`':
            add_token(tokenList, create_token(TOKEN_BITWISE_OPERATOR, "`"));
            break;
        case '~':
            add_token(tokenList, create_token(TOKEN_BITWISE_OPERATOR, "~"));
            break;
        case '^':
            add_token(tokenList, create_token(TOKEN_BITWISE_OPERATOR, "^"));
            break;
        case ';':
            add_token(tokenList, create_token(TOKEN_SEMICOLON, ";"));
            break;
        case '(':
            add_token(tokenList, create_token(TOKEN_PAREN_OPEN, "("));
            break;
        case ')':
            add_token(tokenList, create_token(TOKEN_PAREN_CLOSE, ")"));
            break;
        case '{':
            add_token(tokenList, create_token(TOKEN_BRACE_OPEN, "{"));
            break;
        case '}':
            add_token(tokenList, create_token(TOKEN_BRACE_CLOSE, "}"));
            break;
        case '[':
            add_token(tokenList, create_token(TOKEN_BRACKET_OPEN, "["));
            break;
        case ']':
            add_token(tokenList, create_token(TOKEN_BRACKET_CLOSE, "]"));
            break;
        case ',':
            add_token(tokenList, create_token(TOKEN_COMMA, ","));
            break;
        case ':':
            add_token(tokenList, create_token(TOKEN_OPERATOR, ":"));
            break;
        case '%':
            add_token(tokenList, create_token(TOKEN_OPERATOR, "%"));
            break;
        default:
            add_token(tokenList, create_token(TOKEN_UNKNOWN, "unk"));
            break;
        }
    }
    fclose(file);
    return tokenList;
}