# Rapport du projet de C
<p align="right">VAN BOSSUYT Nicolas<br>JOLY Lucien</p>

## contenu du dossier **data** :

| Nom du fichier |Fonctionnalités|
| -------------  |:--------------|
| client.dat     | contient des données sur les clients fidèles
| stock.dat      | contient des données sur les articles en stock
| user.dat       | contient des données sur le personnel du magasin

## contenu du dossier **source** :
1. ### Dossier **model** 

| Nom du fichier |Fonctionnalités|
| -------------  |:--------------|
| action.c       | _todo_        
| lexer.c        | _todo_        
| model_action.c | _todo_        
| model_view.c   | _todo_        
| model.c        | _todo_        
| user.c         | _todo_        
| view.c         | _todo_       

2. ### Dossier **shop**

| Nom du fichier |Fonctionnalités|
| -------------  |:--------------|
| basket.c       | liste chaînées des articles contenus dans le panier       
| clients.c      | liste chaînée des clients
| session.c      | _todo_        
| shop.c         | _todo_        
| stocks.c       | liste chaînée des articles en stock        
| users.c        | _todo_        

3. ### Dossier **utils**

| Nom du fichier |Fonctionnalités|
| -------------  |:--------------|
| input.c        | _todo_ 
| list.c         | _todo_ 
| logger.c       | _todo_ 
| renderer.c     | _todo_ 
| string.c       | _todo_ 
| terminal.c     | _todo_ 
| variant.c      | _todo_ 
| assert.h       | _todo_ 
| math.h         | _todo_ 

4. ### Dossier **view**

| Nom du fichier             |Fonctionnalités|
| -------------              |:--------------|
| cashier_input_card_id.c    | authentification du client et création d'un nouveau compte client
| cashier_scan_item.c        | lecture et ajout d'articles dans le panier
| cashier_select_what_todo.c | menu client
| home_select_what_todo.c    | menu principal
| return_consigned_bottles.c | retour de bouteilles consignées
| select_client.c            | _todo_ 
| user_login.c               | _todo_ 
| views.h                    | entête des fonctions contenues dans les fichiers .c

5. ### Le fichier main.c

##### note :
> des fichiers '.h' sont aussi presents dans l'aborescence, ils ont les mêmes noms que les fichiers '.c', ceux-ci contiennent les entêtes des fonctions présentes dans les fichiers '.c'.
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