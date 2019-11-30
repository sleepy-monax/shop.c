#include <stdbool.h>
#include <string.h>

#include "utils/string.h"
#include "utils/logger.h"
#include "model/lexer.h"

static const char *type_name[] = {
    "INVALID",
    "BEGIN",
    "END",
    "KEY",
    "VALUE",
    "EOF",
};

const char *token_type_string(Token *tok)
{
    return type_name[tok->type];
}

const char *token_type_string_type(TokenType type)
{
    return type_name[type];
}

int lexer_peek_char(FILE *source)
{
    int c;

    c = fgetc(source);
    ungetc(c, source);

    return c;
}

int lexer_next_char(FILE *source)
{
    return fgetc(source);
}

void lexer_eat_white_space(FILE *source)
{
    int c = lexer_peek_char(source);

    while (is_white_space(c))
    {
        lexer_next_char(source);
        c = lexer_peek_char(source);
    }
}

bool lexer_read_string(FILE *source, Token *tok)
{
    int c = lexer_peek_char(source);

    if (c != '"')
    {
        return false;
    }

    bool escaped = true;

    while ((c != '"' || escaped) && c != EOF)
    {
        if (c == '\\' && !escaped)
        {
            escaped = true;
        }
        else
        {
            strnapd(tok->literal, lexer_next_char(source), VARIANT_SERIALIZED_SIZE);
            escaped = false;
        }

        c = lexer_peek_char(source);
    }

    strnapd(tok->literal, lexer_next_char(source), VARIANT_SERIALIZED_SIZE);
    tok->type = TOKEN_VALUE;

    return true;
}

bool lexer_read_numeric(FILE *source, Token *tok)
{
    int c = lexer_peek_char(source);

    if (!is_numeric(c))
    {
        return false;
    }

    while (is_numeric(c))
    {
        strnapd(tok->literal, lexer_next_char(source), VARIANT_SERIALIZED_SIZE);
        c = lexer_peek_char(source);
    }

    tok->type = TOKEN_VALUE;

    return true;
}

bool lexer_read_keyword_or_key(FILE *source, Token *tok)
{
    int c = lexer_peek_char(source);

    if (!is_letter(c))
    {
        return false;
    }

    while (is_letter(c))
    {
        strnapd(tok->literal, lexer_next_char(source), VARIANT_SERIALIZED_SIZE);
        c = lexer_peek_char(source);
    }

    if (strcmp(tok->literal, "BEGIN") == 0)
    {
        tok->type = TOKEN_BEGIN;
    }
    else if (strcmp(tok->literal, "END") == 0)
    {
        tok->type = TOKEN_END;
    }
    else
    {
        tok->type = TOKEN_KEY;
    }

    return true;
}

bool lexer_read_eof(FILE *source, Token *tok)
{
    int c = lexer_peek_char(source);
    if (c == EOF)
    {
        tok->type = TOKEN_EOF;
        return true;
    }

    return false;
}

Token lexer_next_token(FILE *source)
{
    lexer_eat_white_space(source);

    Token tok = (Token){
        TOKEN_INVALID,
        "",
    };

    if (!(lexer_read_string(source, &tok) ||
          lexer_read_numeric(source, &tok) ||
          lexer_read_keyword_or_key(source, &tok) ||
          lexer_read_eof(source, &tok)))
    {
        log_error("Invalid token '%c'", lexer_peek_char(source));
    }

    return tok;
}