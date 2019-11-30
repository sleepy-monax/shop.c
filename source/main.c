#include "utils/assert.h"
#include "model/lexer.h"
#include "model/model_view.h"
#include "utils/input.h"
#include "view/views.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    FILE *stocks_file = fopen("data/stock.dat", "r");
    FILE *clients_file = fopen("data/client.dat", "r");

    FILE *stocks_save_file = fopen("data/stock.dat.saved", "w");
    FILE *clients_save_file = fopen("data/client.dat.saved", "w");

    StockList *stocks = list_create();
    ClientsList *clients = list_create();

    model_load(stocks_model_create(), stocks, stocks_file);
    model_load(clients_model_create(), clients, clients_file);

    home_select_what_todo(stocks, clients);

    model_save(stocks_model_create(), stocks, stocks_save_file);
    model_save(clients_model_create(), clients, clients_save_file);

    list_destroy(stocks);
    list_destroy(clients);

    return 0;
}
