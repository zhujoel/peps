#include <iostream>

/**
 * TODO: TODO: TODO: TODO: TODO: TODO: TODO: TODO: TODO: TODO: TODO: TODO: TODO: TODO: 
 * 1. estimation des parametres donc il faut une date de début et de fin d'estimation
 *  -> ça va permettre de calculer les matrices sigma et volatilité
 *  -> TODO: split calcul sigma / vol et génériciser adjust sigma
 * 2. simulation -> vecteurs spot (entre 15/05/08 et t (par ex 01/01/09))
 *  -> TODO: remplacer vecteur spot par une matrice
 *  -> on fait le 2eme adjust sur le past (où on multiplie les 3 premieres colonnes par les 3 dernieres)
 *  -> cette matrice past va se coller dans une matrice path
 *  -> path (15/05/08 au 13/05/2016)
 *  -> vecteur 1: date de début et de fin d'estimation (date début past jusqu'à ajd (i.e. t) )
 *  -> vecteur 2: date de past
 *  -> vecteur 3: date de path
 * 3. adaptation du payoff d'océlia -> getMaturity pour renvoyer une proportion de t et etre + précis
 * 4. taux d'intéret étrangers
 * 5. pricing en t
 * 6. tracking error
 */

int main(){
    std::cout << "hello" << std::endl;
}