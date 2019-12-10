---
title: "Rapport d'application de C"
subtitle: "Caisse du colruyt"
author: "Van Bossuyt Nicolas, Joly Lucien"
lang: "fr-BE"
---

# Recherches

Des ticket de plusieur enseigne on ete recupere pour servire d'inspiratio.
Voir figure \ref{fig-ticket-match} et \ref{fig-ticket}.

![Ticket de caisse du match\label{fig-ticket-match}](assets/ticket-match.jpg)

![Ticket de caisse du colruyt\label{fig-ticket}](assets/ticket.jpg)

![Interface utilisateur en mode text\label{fig-interface}](assets/inspiration.jpg)

# Structure du code source et des données

## **data/**

Le dossier data contients les donnes utilser par l'application.

| Nom du fichier | Fonctionnalités                                                                          |
| -------------- | :--------------------------------------------------------------------------------------- |
| client.dat     | La liste des clients qui ont un compte client "extra"                                    |
| stock.dat      | La liste des articles vendu avec leur codebare, nom, categorie, prix et promotion extra. |
| user.dat       | La liste des utilisateur de l'application avec leur nom, prénom, login et mots de passe. |

## client.dat

### Exemple de client :

```
BEGIN
BARECODE 7841
FIRSTNAME "Lara"
LASTNAME "Wenger"
EMAIL "Lara_We@outlook.de"
POINTS 2500
END
```
 - **BEGIN** et **END** sont des "balises" qui indique, lorsqu'on lit le client, de savoir où ses informations commençent et se terminent. De ce fait, toutes les informations du client peuvent être écrit dans n'importe quel ordre.

 - **BARECODE** est le numéro d'identification unique du client, il peut y en avoir de 0 à 9999.

 - **FIRSTNAME**, **LASTNAME** et **EMAIL** sont les informations personnelles du client.

 - **POINTS** est le nombre de points XTRA que le client possède.

## stock.dat

### Exemple d'article en stock :

```
BEGIN
BARECODE 4523
LABEL "Feuilles de bloc"
PRICE 5.000000
CONSIGNED 0.000000
DISCOUNT 15
CATEGORY 1
END
```

 - **BEGIN**, **END** et **BARECODE** fonctionnent commme pour un client dans *client.dat*.
 - **LABEL** est le nom de l'article.
 - **PRICE** est son prix.
 - **CONSIGNED** est la valeur de retour d'un article consigné, si sa valeur est 0, alors l'article n'est pas consigné.
 - **DISCOUNT** est la valeur en pourcentage du rabais.
 - **CATEGORY** est la catégorie de l'article, une valeur entière correspond à une catégorie. Les catégories existantes sont :
    - 0 : UNDEFINDED
    - 1 : OTHER
    - 2 : ALCOHOL
    - 3 : DRINK
    - 4 : FRESH_PRODUCT
    - 5 : COMPUTER
    - 6 : ELECTRONIC
    - 7 : TOYS

## user.dat

### exemple d'utilisateur : 

```
BEGIN
LOGIN "jgera"
FIRSTNAME "Jacqueline"
LASTNAME "Gerard"
PASSWORD 1969958694
ACCESS 2
END
```

 - **BEGIN** et **END** fonctionnent commme pour les fichiers '.dat' précédents.
 - **LOGIN** est le nom d'utilisateur du compte.
 - **FIRSTNAME** et **LASTNAME** sont les informations personnelles du client.
 - **PASSWORD** est un mot de passe crypté, l'utilisateur entrera une chaine de caractère pour s'authentifier dans le programme. Si la chaine entrée, traduite par le programme, correspond au code, alors la personne est authentifiée (si le LOGIN correspond également).
 - **ACCESS** correspond au niveau d'accès du compte, il en existe 3 :
    - 0 : Administrateur
    - 1 : Manager
    - 2 : Caissier

## **source/** :

## **model/** 

| Nom du fichier | Fonctionnalités                                                        |
| -------------- | :--------------------------------------------------------------------- |
| action.c       | Action applicable a tout les models                                    |
| lexer.c        | Lecture des fichier de donnees                                         |
| model.c        | Structure de donnes qui gerent les acces et action d'un model de donné |
| user.c         | Utilisateur du system                                                  |
| view.c         | Affichage d'un model de donné                                          |

## **shop/**

| Nom du fichier | Fonctionnalités                                                               |
| -------------- | :---------------------------------------------------------------------------- |
| basket.c       | Model de donnes et gestion des articles contenus dans le panier d'un clients. |
| clients.c      | Model de donnés et gestion des clients.                                       |
| session.c      | Session d'un client.                                                          |
| stocks.c       | Model de donnés et gestion des articles en stock.                             |
| users.c        | Model de donnés et gestion des utilisateurs de l'application.                 |
| barecode.c     | Gestion des code bare et indentifiant unique.                                 |

## **utils/**

| Nom du fichier | Fonctionnalités                                                                                           |
| -------------- | :-------------------------------------------------------------------------------------------------------- |
| input.c        | Entree de l'utilisateur                                                                                   |
| list.c         | Implementation generic d'un liste doublement chainer                                                      |
| logger.c       | Journal de bord de l'application                                                                          |
| renderer.c     | Gestionaire d'afichage de l'application                                                                   |
| string.c       | Utilitaire pour la manipulation de chaine de caracter UTF-8                                               |
| terminal.c     | Interaction de bas niveau avec le terminal                                                                |
| variant.c      | Type de donnée generic utiliser pour la comunication entre l'interface utilisateur et le model de donnée. |
| assert.h       | Utilitaire de debuggage.                                                                                  |
| math.h         | Fonction matematique suplementaire                                                                        |

## **view/**

| Nom du fichier             | Fonctionnalités                                                   |
| -------------------------- | :---------------------------------------------------------------- |
| cashier_input_card_id.c    | Authentification du client et création d'un nouveau compte client |
| cashier_scan_item.c        | Lecture et ajout d'articles dans le panier                        |
| cashier_select_what_todo.c | Menu client                                                       |
| home_select_what_todo.c    | Menu principal                                                    |
| return_consigned_bottles.c | Retour de bouteilles consignées                                   |
| user_login.c               | Interaface de connection de l'utilisateur.                        |
| views.h                    | Entête des fonctions contenues dans les fichiers **view/*.c**     |

## main.c

> Des fichiers '.h' sont aussi presents dans l'aborescence, ils ont les mêmes noms que les fichiers '.c', ceux-ci contiennent les entêtes des fonctions présentes dans les fichiers '.c'.
