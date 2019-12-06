#pragma once

#include "shop/stocks.h"
#include "shop/clients.h"
#include "shop/users.h"
#include "shop/session.h"

void user_login(UsersList *users, StockList *stocks, ClientsList *clients);

void home_select_what_todo(User *user, StockList *stocks, ClientsList *clients);

/* --- Manager -------------------------------------------------------------- */

void manager_select_what_todo(User *user, StockList *stock, ClientsList *clients);

void mamager_manage_clients(ClientsList *clients);

void manager_manage_stocks(StockList *stock);

/* --- Cashier -------------------------------------------------------------- */

void cashier_select_what_todo(Session *session, StockList *stocks, ClientsList *clients);

void cashier_scan_items(Session *session, StockList *stock);

void cashier_return_consigned_bottles(Session *session, StockList *stock);

void cashier_input_card_id(Session *session, ClientsList *clients);

void cashier_create_card_id(Session *session, ClientsList *clients);