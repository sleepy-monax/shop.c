#pragma once

#include <stdio.h>
#include "utils/variant.h"

typedef enum
{
    TOKEN_INVALID,
    TOKEN_BEGIN,
    TOKEN_END,
    TOKEN_KEY,
    TOKEN_VALUE,
    TOKEN_EOF,
} TokenType;

typedef struct
{
    int ln;
    int col;

    TokenType type;
    char literal[VARIANT_SERIALIZED_SIZE];
} Token;

typedef struct
{
    int ln;
    int col;

    FILE *source;
} Lexer;

const char *token_type_string(Token *tok);

const char *token_type_string_type(TokenType type);

Token lexer_next_token(Lexer *lex);
