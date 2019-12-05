#include "view/views.h"

void *load_data(Model model, void *data, const char *path)
{
    char path_with_sufix[256];
    snprintf(path_with_sufix, 256, "%s.saved", path);

    FILE *fdat = fopen(path_with_sufix, "r");
    if (!fdat)
    {
        fdat = fopen(path, "r");
    }

    model_load(model, data, fdat);
    fclose(fdat);

    return data;
}

void save_data(Model model, void *data, const char *path)
{
    char path_with_sufix[256];
    snprintf(path_with_sufix, 256, "%s.saved", path);

    FILE *stocks_save_file = fopen(path_with_sufix, "w");
    model_save(model, data, stocks_save_file);
    list_destroy(data);
    fclose(stocks_save_file);
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    UsersList *users = load_data(users_model_create(), list_create(), "data/user.dat");
    ClientsList *clients = load_data(clients_model_create(), list_create(), "data/client.dat");
    StockList *stocks = load_data(stocks_model_create(), list_create(), "data/stock.dat");

    user_login(users, stocks, clients);

    save_data(clients_model_create(), clients, "data/client.dat");
    save_data(stocks_model_create(), stocks, "data/stock.dat");
    save_data(users_model_create(), users, "data/user.dat");

    return 0;
}
