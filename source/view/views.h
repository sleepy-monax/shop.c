#pragma once

#include "shop/stocks.h"
#include "shop/clients.h"
#include "shop/session.h"

void user_login();

void home_select_what_todo(StockList *stocks);

/* --- Manager -------------------------------------------------------------- */

void manager_select_what_todo(StockList *stock, ClientsList *clients);

void mamager_manage_clients(ClientsList *clients);

void manager_manage_stocks(StockList *stock);

/* --- Cashier -------------------------------------------------------------- */

/*
    On demand au caissier ce qu'il souhaite faire.

    Parametres
    ----------
    stocks: liste des stocks
    clients: liste des clients

    Return
    ------
    rien.

    Implementation
    --------------
    Joly Lucien 
*/
//void cashier_select_what_todo(StockList *stock, ClientsList *clients);

// while cashier_select_what_todo() isn't made yet
void cashier_select_what_todo(StockList *stock);

/*
    Les caissier scans les items du clients.

    Parametres
    ----------
    session: session du clients
    stocks: liste des stocks

    Return
    ------
    rien.

    Implementation
    --------------
    Joly Lucien 
*/
void cashier_scan_items(Session *session, StockList *stock);

/*
    Le caissier recuper les bouteille consigner du clients.

    Parametres
    ----------
    session: session du clients
    stocks: liste des stocks

    Return
    ------
    rien.

    Implementation
    --------------
    Joly Lucien 
*/
//void cashier_return_consigned_bottles(Session *session, StockList *stock);


// while cashier_return_consigned_bottles() isn't made yet
void cashier_return_consigned_bottles(StockList *stock);

/*
    Le caissier demand le numero de carte du client.

    Parametres
    ----------
    session: session du clients
    clients: liste des clients

    Return
    ------
    rien.

    Implementation
    --------------
    Joly Lucien 
*/
void cashier_input_card_id(Session *session, ClientsList *clients);

/*
    Le caissier creer un nouvelle cart pour le clients

    Parametres
    ----------
    session: session du clients
    clients: liste des clients

    Return
    ------
    rien.

    Implementation
    --------------
    Joly Lucien 
*/
void cashier_create_card_id(Session *session, ClientsList *clients);
