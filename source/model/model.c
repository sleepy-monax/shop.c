#include <string.h>
#include <stdbool.h>

#include "utils/logger.h"
#include "utils/assert.h"
#include "model/lexer.h"
#include "model/model.h"

int model_get_column(Model model, const char *name)
{
    for (int i = 0; i < model.column_count(); i++)
    {
        if (strcmp(name, model.column_name(i, ROLE_DATA)) == 0)
        {
            return i;
        }
    }

    return -1;
}

typedef enum
{
    MODEL_LOAD_BEGIN,
    MODEL_LOAD_KEY,
    MODEL_LOAD_VALUE,
} ModelLoadState;

bool model_load_expect(Token *tok, TokenType expected)
{
    if (tok->type != expected)
    {
        log_error("Parser: ln%d, col%d: Expected token %s got token %s '%s'",
                  tok->ln,
                  tok->col,
                  token_type_string_type(expected),
                  token_type_string(tok),
                  tok->literal);

        return false;
    }

    return true;
}

void model_load(Model model, void *data, FILE *source)
{
    ModelLoadState state = MODEL_LOAD_BEGIN;
    int row = -1;
    int column = -1;

    Lexer lex = {0};
    lex.ln = 1;
    lex.source = source;

    Token tok = lexer_next_token(&lex);
    do
    {
        if (state == MODEL_LOAD_BEGIN)
        {
            if (model_load_expect(&tok, TOKEN_BEGIN))
            {
                row = model.row_create(data);
                state = MODEL_LOAD_KEY;
            }
        }
        else if (state == MODEL_LOAD_KEY)
        {
            if (tok.type == TOKEN_KEY)
            {
                column = model_get_column(model, tok.literal);

                if (column == -1)
                {
                    log_error("Loader: ln%d, col%d: Le modele ne contient pas la colonne %s!", tok.ln, tok.col, tok.literal);
                }

                state = MODEL_LOAD_VALUE;
            }
            else if (tok.type == TOKEN_END)
            {
                state = MODEL_LOAD_BEGIN;
            }
            else
            {
                model_load_expect(&tok, TOKEN_KEY);
            }
        }
        else if (state == MODEL_LOAD_VALUE)
        {
            if (model_load_expect(&tok, TOKEN_VALUE))
            {
                Variant value = variant_deserialize(tok.literal);

                if (value.type == model.column_type(column, ROLE_DATA))
                {
                    model.set_data(data, row, column, value, ROLE_DATA);
                }
                else
                {
                    log_error("Loader: ln%d, col%d: Le type de la colonne(%s) dans ne corespond pas avec le type colonne du model! (%d!=%d)",
                              tok.ln,
                              tok.col,
                              model.column_name(column, ROLE_DATA),
                              value.type,
                              model.column_type(column, ROLE_DATA));
                }

                state = MODEL_LOAD_KEY;
            }
        }
        else
        {
            ASSERT_NOT_REACHED();
        }

        tok = lexer_next_token(&lex);
    } while (tok.type != TOKEN_EOF);
}

void model_save(Model model, void *data, FILE *destination)
{
    for (int row = 0; row < model.row_count(data); row++)
    {
        fprintf(destination, "BEGIN\n");

        for (int column = 0; column < model.column_count(); column++)
        {
            char serialied_value[VARIANT_SERIALIZED_SIZE];

            variant_serialize(model.get_data(data, row, column, ROLE_DATA), serialied_value);

            fprintf(destination, "%s %s\n", model.column_name(column, ROLE_DATA), serialied_value);
        }

        fprintf(destination, "END\n\n");
    }
}

Variant model_get_data_with_access(Model model, void *data, int row, int column, User *user, ModelRole role)
{
    if (user->access <= model.read_access(data, row, column, user))
    {
        return model.get_data(data, row, column, role);
    }
    else
    {
        return vstring("####");
    }
}

void model_set_data_with_access(Model model, void *data, int row, int column, Variant value, User *user)
{
    if (user->access <= model.read_access(data, row, column, user))
    {
        return model.set_data(data, row, column, value, ROLE_DATA);
    }
}
