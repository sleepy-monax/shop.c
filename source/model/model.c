#include <string.h>

#include "utils/logger.h"
#include "utils/assert.h"
#include "model/lexer.h"
#include "model/model.h"

int model_get_column(Model model, const char *name)
{
    for (int i = 0; i < model.column_count(); i++)
    {
        if (strcmp(name, model.column_name(i)) == 0)
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

void model_load_expect(Token *tok, TokenType expected)
{
    if (tok->type != expected)
    {
        log_error("Expected token %s got token %s '%s'", token_type_string_type(expected), token_type_string(tok), tok->literal);
        ASSERT_NOT_REACHED();
    }
}

void model_load(Model model, void *data, FILE *source)
{
    ModelLoadState state = MODEL_LOAD_BEGIN;
    int row = -1;
    int column = -1;

    Token tok = lexer_next_token(source);
    while (tok.type != TOKEN_EOF && tok.type != TOKEN_INVALID)
    {
        if (state == MODEL_LOAD_BEGIN)
        {
            model_load_expect(&tok, TOKEN_BEGIN);
            row = model.row_create(data);
            state = MODEL_LOAD_KEY;
        }
        else if (state == MODEL_LOAD_KEY)
        {
            if (tok.type == TOKEN_KEY)
            {
                column = model_get_column(model, tok.literal);

                if (column == -1)
                {
                    log_error("Le modele ne contient pas la colonne %s!", tok.literal);
                    ASSERT_NOT_REACHED();
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
            model_load_expect(&tok, TOKEN_VALUE);

            Variant value = variant_deserialize(tok.literal);

            if (value.type == model.column_type(column))
            {
                model.set_data(data, row, column, value);
            }
            else
            {
                log_error("Le type de la collone(%s) dans ne corespond pas avec le type collone du model! (%d!=%d)",
                          model.column_name(column),
                          value.type, model.column_type(column));
                ASSERT_NOT_REACHED();
            }

            state = MODEL_LOAD_KEY;
        }
        else
        {
            ASSERT_NOT_REACHED();
        }

        tok = lexer_next_token(source);
    }
}

void model_save(Model model, void *data, FILE *destination)
{
    for (int row = 0; row < model.row_count(data); row++)
    {
        fprintf(destination, "BEGIN\n");

        for (int column = 0; column < model.column_count(); column++)
        {
            char serialied_value[VARIANT_SERIALIZED_SIZE];

            variant_serialize(model.get_data(data, row, column), serialied_value);

            fprintf(destination, "%s %s\n", model.column_name(column), serialied_value);
        }

        fprintf(destination, "END\n\n");
    }
}