#include "utils/input.h"
#include "view/views.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    FILE *stocks_file = fopen("data/stock.dat", "r");
    FILE *clients_file = fopen("data/client.dat", "r");

    StockList *stocks = stocks_create(stocks_file);
    ClientsList *clients = clients_create(clients_file);

    setup_terminal_for_user_input();

    select_what_todo(stocks);

    restore_terminal_after_user_input();

    printf("Fin du programme ...\n");

    stocks_destroy(stocks);

    clients_destroy(clients);

    return 0;
}
