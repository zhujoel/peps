#pragma once

#include <string>
#include "pnl/pnl_vector.h"

/**
 * @brief Sépare une phrase en un tableau de mots. 
 * 
 * @param output[out] Tableau de string.
 * @param line String à séparer.
 * @param delimiter Délimiteur entre les mots.
 */
void split(std::string * const output, const std::string &line, char delimiter);
/**
 * @brief Arrondi un vecteur de nombre à n décimals.
 * 
 * @param vect[out] Vecteur à arrondir
 * @param n Nombres après la virgule
 */
void trunc(PnlVect * const vect, int n);
/**
 * @brief Détermine si tous les valeurs du vecteur sont positives.
 * 
 * @param vect Vecteur de valeurs.
 * @return true Si toutes les valeurs sont positives
 * @return false S'il y a une valeur négative.
 */
bool are_all_positive(const PnlVect * const vect);
/**
 * @brief Détermine s'il y a un mot 'null' dans un string.
 * 
 * @param array String à parser.
 * @param indexes Indice du mot qu'on souhaite voir.
 * @return true Si aucun des mots sont 'null'.
 * @return false Si le mot d'un des indices est 'null'.
 */
bool has_no_index_value_null(const std::string * const array, const PnlVectInt * const indexes);
/**
 * @brief Détermine le premier indice du vecteur qui est supérieur ou égal à une valeur donnée.
 * On considère que le vecteur est ordonné en ordre croissant.
 * 
 * @param vect Vecteur de valeurs.
 * @param value Valeur qu'on souhaite vérifier si gte.
 * @return int Premier indice supérieur ou égal.
 */
int first_index_gte(const PnlVect * const vect, double value);
/**
 * @brief Print un objet PnlVect dans un stream.
 * 
 * @param stm stream
 * @param vect vecteur
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream &stm, const PnlVect * const vect);