# Projet PEPS 2021

## Auteurs
* Kaouthar HRAIM
* Mohamed Mahmoud KHOUNA
* Philippe MORAIS
* Eloïse VRANA
* Joël ZHU

# Etat actuel du projet 

* Intégration en cours d'un pricing pour Océlia prenant en compte le taux de change.

* Validation d'un pricing de call sur action étrangère avec strike en devise domestique (QuantoOption).

* Architecture de tests unitaires (google-test) sur payoff de Océlia et une bibliothèque interne de gestion de dates.

## Programme principal

* Pricing d'un call sur action étrangère avec strike en devise domestique.

* Payoff du produit Océlia avec des donneés simulées (Diffusion B-S ne fonctionne pas)

# Compilation

## Programme principal

mkdir build

cd build

cmake -DCMAKE_PREFIX_PATH=<location_lib_pnl> .. (e.g. : cmake -DCMAKE_PREFIX_PATH=/home/zhujo/Downloads/pnl/build/lib/cmake/pnl ..)

make

./main

## Tests unitaires

mkdir build

cd build

cmake -DCMAKE_PREFIX_PATH=<location_lib_pnl> .. (e.g. : cmake -DCMAKE_PREFIX_PATH=/home/zhujo/Downloads/pnl/build/lib/cmake/pnl ..)

ctest -V
