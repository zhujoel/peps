# Project PEPS 2021

Ensimag 2020-2021 - 5MMPEPS4 Projet évaluation de produits structurés

Calcule le prix d'un produit structuré complexe (cf. rendus/ocelia.pdf) en utilisant les algorithmes de Monte-Carlo et le modèle de Black-Scholes.

Codé en C++.

**VOIR LE DOSSIER rendus/ POUR UNE DOCUMENTATION DÉTAILLÉE**

## Auteurs

* Kaouthar HRAIM
* Mohamed Mahmoud KHOUNA
* Philippe MORAIS
* Eloïse VRANA
* Joël ZHU

## Tuteur 

* Jérôme LELONG

## How to run

* Pricing d'un call sur action étrangère avec strike en devise domestique.

* Payoff du produit Océlia avec des donneés simulées (Diffusion B-S ne fonctionne pas)

# Compilation

## Programme principal

You need to download and compile the library PNL: https://pnlnum.github.io/pnl/

```
mkdir build/
cd build/
cmake -DCMAKE_PREFIX_PATH=<location_lib_pnl> ..
(Example: cmake -DCMAKE_PREFIX_PATH=/home/zhujo/Downloads/pnl/build/lib/cmake/pnl ..)
make
./main (all | next) [<output_filepath> <delta_filepath>]
```

Explication de la commande ./main :
* 'all': simule la trajectoire du début à la fin du produit.
* 'next': simule la trajectoire à partir des dernières données du fichier de output. Dans le cas du 'next', les fichiers 'output' et 'delta' doivent être indiqués.
* 'output_filepath': Correspond au fichier où écrire les données calculées. Dans le cas du 'all', il est optionnel. S'il n'est pas indiqué, les données sont écrites dans la sortie standard. Dans le cas du 'next', il doit êêtre indiqué car le programme doit lire le contenu du fichier.
* 'delta_filepath': Correspond au fichier où écrire les données deltas. On impose les mêmes contraintes que pour le fichier output. De plus, si on souhaite écrire dans le fichier delta, il faut obligatoirement indiquer le fichier output également.

Exemples :

* ``` ./main all ```
* ``` ./main all ../this_is_my_output_file ```
* ``` ./main all ../this_is_my_output_file ../this_is_my_delta_file ```
* ``` ./main next ../mandatory_output_file ../mandatory_delta_file ```

# Organisation du dépôt

* ``` data/ ``` : Fichiers de données lu par le programme. Données de marché des sous-jacents et les dates utilisées.
* ``` docs/ ``` : Configuration Doxygen pour générer la documentation.
* * ``` doxygen docs/Doxyfile ```
* ``` gitstats/ ``` : Statistiques sur le dépôt git.
* ``` include/ ``` : Fichiers header.
* ``` output/ ``` : Simulation fait en avance. Contient un .png du graphe et le résultat du profiler.
* ``` rendus/ ``` : Documents .pdf de documentation.
* ``` include/ ``` : Fichiers source.
* ``` tests/ ``` : Tests unitaires.
* * ``` ctest -V ```
