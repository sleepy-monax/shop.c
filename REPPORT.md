# Rapport du projet de C
<p align="right">VAN BOSSUYT Nicolas<br>JOLY Lucien</p>

## contenu du dossier **data** :
- client.dat
- stock.dat
- user.dat

## contenu du dossier **source** :
1. ### Dossier **model** 
    - action.c
    - lexer.c
    - model_action.c
    - model_view.c
    - model.c
    - user.c
    - view.c

2. ### Dossier **shop**
    - basket.c
    - clients.c
    - session.c
    - shop.c
    - stocks.c
    - users.c

3. ### Dossier **utils**
    - input.c
    - list.c
    - logger.c
    - renderer.c
    - string.c
    - terminal.c
    - variant.c
    - assert.h
    - math.h

4. ### Dossier **view**
    - cashier_input_card_id.c
    - cashier_scan_item.c
    - cashier_select_what_todo.c
    - home_select_what_todo.c
    - return_consigned_bottles.c
    - select_client.c
    - user_login.c
    - views.h
5. ### Le fichier main.c

## des fichiers '.h' sont aussi presents dans l'aborescence, ils ont les mêmes noms que les fichiers '.c', ceux-ci contiennent les entêtes des fonctions présentes dans les fichiers '.c'.
<br>

# contenu des fichiers '.dat' :

1. ### <u>client.dat :</u>

#### exemple de client :
```
BEGIN
BARECODE 7841
FIRSTNAME "Lara"
LASTNAME "Wenger"
EMAIL "Lara_We@outlook.de"
POINTS 2500
END
```
#### BEGIN et END sont des "balises" qui indique, lorsqu'on lit le client, de savoir où ses informations commençent et se terminent. De ce fait, toutes les informations du client peuvent être écrit dans n'importe quel ordre.

#### BARECODE est le numéro d'identification unique du client, il peut y en avoir de 0 à 9999.

#### FIRSTNAME, LASTNAME et EMAIL sont les informations personnelles du client.

#### POINTS est le nombre de points XTRA que le client possède.
<br>

2. ### <u>stock.dat</u> :

#### exemple d'article en stock :
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

#### BEGIN, END et BARECODE fonctionnent commme pour un client dans *client.dat*.
#### LABEL est le nom de l'article.
#### PRICE est son prix.
#### CONSIGNED est la valeur de retour d'un article consigné, si sa valeur est 0, alors l'article n'est pas consigné.
#### DISCOUNT est la valeur en pourcentage du rabais.
#### CATEGORY est la catégorie de l'article, une valeur entière correspond à une catégorie. Les catégories existantes sont :
<dt>0 : UNDEFINDED</dt>
<dt>1 : OTHER</dt>
<dt>2 : ALCOHOL</dt>
<dt>3 : DRINK</dt>
<dt>4 : FRESH_PRODUCT</dt>
<dt>5 : COMPUTER</dt>
<dt>6 : ELECTRONIC</dt>
<dt>7 : TOYS</dt>
<br>

3. ### <u>user.dat</u>

#### exemple d'utilisateur : 
```
BEGIN
LOGIN "jgera"
FIRSTNAME "Jacqueline"
LASTNAME "Gerard"
PASSWORD 1969958694
ACCESS 2
END
```

#### BEGIN et END fonctionnent commme pour les fichiers '.dat' précédents.
#### LOGIN est le nom d'utilisateur du compte.
#### FIRSTNAME et LASTNAME sont les informations personnelles du client.
#### PASSWORD est un mot de passe crypté, l'utilisateur entrera une chaine de caractère pour s'authentifier dans le programme. Si la chaine entrée, traduite par le programme, correspond au code, alors la personne est authentifiée (si le LOGIN correspond également).
#### ACCESS correspond au niveau d'accès du compte, il en existe 3 :
<dt>0 : Administrateur</dt>
<dt>1 : Manager</dt>
<dt>2 : Caissier</dt>