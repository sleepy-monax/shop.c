#include "utils/input.h"
#include "view/views.h"

#include "model/model_view.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    FILE *stocks_file = fopen("data/stock.dat", "r");
    FILE *clients_file = fopen("data/client.dat", "r");

    StockList *stocks = stocks_create(stocks_file);
    ClientsList *clients = clients_create(clients_file);

    model_view(clients_model_create(), clients);

    home_select_what_todo(stocks, clients);

    stocks_destroy(stocks);
    clients_destroy(clients);

    return 0;
}
