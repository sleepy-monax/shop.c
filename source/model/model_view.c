#include <stdio.h>

#include "utils/assert.h"
#include "model/model_view.h"

void model_view(Model model, void *data)
{
    for (int i = 0; i < model.column_count(); i++)
    {
        printf("%s ", model.column_name(i));
    }

    printf("\n");

    for (int row = 0; row < model.row_count(data); row++)
    {
        for (int column = 0; column < model.column_count(); column++)
        {
            printf("%s  ", model.get_data(data, row, column).as_string);
        }

        printf("\n");
    }

    ASSERT_NOT_REACHED();
}
