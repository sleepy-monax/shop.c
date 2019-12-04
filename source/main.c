#include "view/views.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    FILE *stocks_file = fopen("data/stock.dat.saved", "r");
    if (!stocks_file)
    {
        stocks_file = fopen("data/stock.dat", "r");
    }

    StockList *stocks = list_create();
    model_load(stocks_model_create(), stocks, stocks_file);
    fclose(stocks_file);

    FILE *clients_file = fopen("data/client.dat.saved", "r");
    if (!clients_file)
    {
        clients_file = fopen("data/client.dat", "r");
    }

    ClientsList *clients = list_create();
    model_load(clients_model_create(), clients, clients_file);
    fclose(clients_file);

    home_select_what_todo(stocks, clients);

    FILE *stocks_save_file = fopen("data/stock.dat.saved", "w");
    model_save(stocks_model_create(), stocks, stocks_save_file);
    list_destroy(stocks);
    fclose(stocks_save_file);

    FILE *clients_save_file = fopen("data/client.dat.saved", "w");
    model_save(clients_model_create(), clients, clients_save_file);
    list_destroy(clients);
    fclose(clients_save_file);

    return 0;
}
