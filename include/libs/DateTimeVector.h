#pragma once

#include "libs/DateTime.h"
#include "pnl/pnl_vector.h"
#include <vector>
#include "pnl/pnl_matrix.h"

/**
 * @brief Parse un fichier de dates.
 * 
 * @param dates[out] Les dates parsées.
 * @param fileName Nom du fichier à parser.
 * @param nbDates Nombre de dates dans le fichire.
 * @param delimiter Délimiteur des dates. (e.g. 1993/01/01, le délimiteur est '/').
 */
void parse_dates_file(std::vector<DateTime*> &dates, const std::string &fileName, int nbDates, char delimiter);
/**
 * @brief Calcule les indices des dates contenus dans un ensemble.
 * 
 * @param indices[out] Les indices calculés.
 * @param all_dates Ensemble contenant toutes les dates.
 * @param dates_subset Sous-ensemble qui contient les dates dont on souhaite calculer les indices.
 */
void calcul_indices_dates(PnlVectInt * const indices, const std::vector<DateTime*> &all_dates, const std::vector<DateTime*> &dates_subset); // LES DATES DOIVENT ETRE DANS L'ORDRE
/**
 * @brief Calcule l'indice d'une date contenu dans un ensemble.
 * 
 * @param all_dates Ensemble contenant toutes les dates.
 * @param date Date dont on souhaite calculer l'indice.
 * @return int L'indice de la date dans l'ensemble.
 */
int get_indice_from_date(const std::vector<DateTime*> &all_dates, const DateTime * const date);
/**
 * @brief Calcule l'indice d'une date contenu dans un ensemble. Recherche dichotomique récursive dans un vecteur.
 * 
 * @param all_dates 
 * @param date 
 * @param start 
 * @param end 
 * @return int L'indice de la date dans l'ensemble.
 */
int get_indices_from_date_dichotomic(const std::vector<DateTime*> &all_dates, const DateTime * const date, int start, int end);
/**
 * @brief Détermine les dates en commun entre deux vecteurs de dates.
 * 
 * @param v1 Vecteur date 1.
 * @param v2 Vecteur date 2.
 * @return std::vector<DateTime*> Retourne le vecteur contenant seulement les dates en commun.
 */
std::vector<DateTime*> same_dates(const std::vector<DateTime*> &v1, const std::vector<DateTime*> &v2);
/**
 * @brief Récupère les prix qui nous intéressent dans un vecteur de prix en fonction des dates.
 * 
 * @param prices[out] Prix qui nous intéressent.
 * @param allDates Toutes les dates.
 * @param subset Dates à laquelles les prix nous intéressent.
 * @param allPrices Tous les prix. Il doit y avoir un même nombre de prix que de dates dans allDates.
 */
void get_prices_from_date(PnlVect * const prices, const std::vector<DateTime*> &allDates, const std::vector<DateTime*> &subset, const PnlVect * const allPrices);
/**
 * @brief Récupère les prix qui nous intéressent dans une matrice de prix en fonction des dates.
 * La différence avec l'autre fonction est qu'ici allPrices peut contenir plusieurs prix par date alors que l'autre est seulement le path pour d'un seul prix.
 * 
 * @param prices[out] Prix qui nous intéressent.
 * @param allDates Toutes les dates.
 * @param subset Dates à laquelles les prix nous intéressent.
 * @param allPrices Tous les prix. Il doit y avoir un même nombre de prix que de dates dans allDates.
 */
void get_prices_from_date(PnlMat * const prices, const std::vector<DateTime*> &allDates, const std::vector<DateTime*> &subset, const PnlMat * const allPrices);
/**
 * @brief Récupère les dates entre deux dates.
 * 
 * @param from_to[out] Les dates entre les deux dates indiquées.
 * @param allDates Toutes les dates.
 * @param from Date de début.
 * @param to Date de fin.
 */
void from_date_to_date(std::vector<DateTime*> &from_to, const std::vector<DateTime*> &allDates, const DateTime * const from, const DateTime * const to);
/**
 * @brief Libère la mémoire d'un vecteur de pointeurs de date.
 * 
 * @param list Vecteur de pointeur.
 */
void delete_date_vector(std::vector<DateTime*> &list);