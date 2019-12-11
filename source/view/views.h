#pragma once

#include "shop/basket.h"
#include "shop/clients.h"
#include "shop/stocks.h"
#include "shop/users.h"

void user_login(UsersList *users, StockList *stocks, ClientsList *clients);

void home_select_what_todo(User *user, UsersList *users, StockList *stocks, ClientsList *clients);

void manager_select_what_todo(User *user, UsersList *users, StockList *stock, ClientsList *clients);

void cashier_select_what_todo(User *user, Basket *basket, StockList *stocks, ClientsList *clients);

void cashier_scan_items(Basket *basket, StockList *stock);

void cashier_return_consigned_bottles(Basket *basket, StockList *stock);

Client *cashier_input_card_id(ClientsList *clients);