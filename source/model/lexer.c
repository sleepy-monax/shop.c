#include <stdbool.h>
#include <string.h>

#include "utils/string.h"
#include "utils/logger.h"
#include "utils/assert.h"
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

int lexer_peek_char(Lexer *lex)
{
    int c;

    c = fgetc(lex->source);
    ungetc(c, lex->source);

    return c;
}

int lexer_next_char(Lexer *lex)
{
    int c = fgetc(lex->source);

    if (c == '\n')
    {
        lex->ln++;
        lex->col = 0;
    }
    else
    {
        lex->col++;
    }

    return c;
}

void lexer_eat_white_space(Lexer *lex)
{
    int c = lexer_peek_char(lex);

    while (is_white_space(c))
    {
        lexer_next_char(lex);
        c = lexer_peek_char(lex);
    }
}

bool lexer_read_string(Lexer *lex, Token *tok)
{
    int c = lexer_peek_char(lex);

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
            strnapd(tok->literal, lexer_next_char(lex), VARIANT_SERIALIZED_SIZE);
            escaped = false;
        }

        c = lexer_peek_char(lex);
    }

    strnapd(tok->literal, lexer_next_char(lex), VARIANT_SERIALIZED_SIZE);
    tok->type = TOKEN_VALUE;

    return true;
}

bool lexer_read_numeric(Lexer *lex, Token *tok)
{
    int c = lexer_peek_char(lex);

    if (!is_numeric(c))
    {
        return false;
    }

    while (is_numeric(c))
    {
        strnapd(tok->literal, lexer_next_char(lex), VARIANT_SERIALIZED_SIZE);
        c = lexer_peek_char(lex);
    }

    tok->type = TOKEN_VALUE;

    return true;
}

bool lexer_read_keyword_or_key(Lexer *lex, Token *tok)
{
    int c = lexer_peek_char(lex);

    if (!is_letter(c))
    {
        return false;
    }

    while (is_letter(c))
    {
        strnapd(tok->literal, lexer_next_char(lex), VARIANT_SERIALIZED_SIZE);
        c = lexer_peek_char(lex);
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

bool lexer_read_eof(Lexer *lex, Token *tok)
{
    int c = lexer_peek_char(lex);

    if (c == EOF)
    {
        tok->type = TOKEN_EOF;
        return true;
    }

    return false;
}

Token lexer_next_token(Lexer *lex)
{
    lexer_eat_white_space(lex);

    Token tok = (Token){
        lex->ln,
        lex->col,
        TOKEN_INVALID,
        "",
    };

    if (!(lexer_read_string(lex, &tok) ||
          lexer_read_numeric(lex, &tok) ||
          lexer_read_keyword_or_key(lex, &tok) ||
          lexer_read_eof(lex, &tok)))
    {
        log_error("Lexer: ln%d, col%d: Unexpected codepoint %d '%c'", lex->ln, lex->col, lexer_peek_char(lex), lexer_peek_char(lex));
        lexer_next_char(lex);
    }

    return tok;
}