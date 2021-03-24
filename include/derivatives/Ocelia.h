#pragma once

#include "derivatives/IDerivative.h"
#include "libs/DateTimeVector.h"
#include "pnl/pnl_matrix.h"
#include <vector>
#include "libs/InterestRate.h"

/**
 * @brief Gère le produit Océlia.
 * 
 */
class Ocelia : public IDerivative{
    public:
        /**
         * @brief Vecteur de tous les t précalculés qui nous intéressent. 
         * C'est-à-dire les 49 dates de constatations décrites dans l'analyse financière.
         * 
         */
        PnlVect *computed_ti_;
        /**
         * @brief Valeur liquidative initiale du produit.
         * 
         */
        double valeur_liquidative_initiale_;
        /**
         * @brief Classe qui gère les taux d'intérêts. Sert pour ajuster les prix et sigma.
         * 
         */
        InterestRate *rates_;
        /**
         * @brief Année de payoff du produit. 4 <= N <= 8.
         * 
         */
        int annee_payoff_;
        /**
         * @brief Nombre de sous-jacents du produit.
         * 
         */
        int nb_sous_jacents_;
        /**
         * @brief Indices des dates semestrielles dans le path.
         * 
         */
        PnlVectInt *indices_dates_semestrielles_;
        /**
         * @brief Indices des dates de valeurs N ans dans le path.
         * 
         */
        PnlVectInt *indices_dates_valeurs_n_ans_;
        /**
         * @brief Vecteur qui stocke les valeurs N ans intermédiaires. Nécessaires pour le calcul du payoff.
         * 
         */
        PnlVect *valeurs_n_ans_;
        /**
         * @brief Valeurs initiales des sous-jacents (à l'année 0).
         * 
         */
        PnlVect *valeurs_initiales_;
        /**
         * @brief Valeurs de l'effet nouveau départ (soit égales aux valeurs initiales soit 90% de ça).
         * 
         */
        PnlVect *nouveau_depart_;
        /**
         * @brief Vecteur qui stocke les performances à l'année N. Nécessaires pour le calcul du payoff.
         * 
         */
        PnlVect *perfs_;

        /**
         * @brief Construct a new Ocelia object
         * 
         * @param T 
         * @param size 
         * @param nb_sous_jacents 
         * @param valeur_liquidative_initiale 
         * @param computed_ti_ 
         * @param rates 
         */
        Ocelia(double T, int size, int nb_sous_jacents, double valeur_liquidative_initiale, PnlVect * const computed_ti_, InterestRate * const rates);
        /**
         * @brief Destroy the Ocelia object
         * 
         */
        ~Ocelia();

        void adjust_sigma(PnlMat * const sigma) const;
        void adjust_past(PnlMat * const past) const;
        void adjust_spot(PnlVect * const spot, double t) const;
        double get_annee_payoff() const;

        /**
         * @brief Initialise les vecteurs des indices des dates de constatation et des dates de valeurs N ans.
         * 
         * @param all_dates Toutes les dates utilisées du début à la fin du produit.
         * @param dates_semestrielles Dates semestrielles qui nous intéressent. Doit être inclu dans all_dates.
         * @param dates_valeurs_n_ans Dates des valeurs N ans qui nous intéressent. Doit être inclu dans all_dates.
         */
        void init_indices(const std::vector<DateTime*> &all_dates, const std::vector<DateTime*> &dates_semestrielles, const std::vector<DateTime*> &dates_valeurs_n_ans);

        /**
         * @brief Calcule le prix d'un sous-jacent à une date donnée en retournant en devise étranger et en actualisant.
         * 
         * @param path Path contenant tous les prix.
         * @param date_idx Indice de la date qui nous intéresse dans la matrice path.
         * @param idx Indice du produit qui nous intéresse.
         * @return double Prix du sous-jacent converti en devise étrangère et actualisé.
         */
        double get_foreign_index_market_value(const PnlMat* path, int date_idx, int idx) const;
        /**
         * @brief Calcule la performance moyenne du panier comme décrite dans l'analyse financière.
         * 
         * @throws std::invalid_argument L'argument idx doit être entre 0 et 3 pour Océlia (entre 4 et 7 les indices correspondent aux zéro-coupons étrangers).
         * @param path Path contenant tous les prix.
         * @return double Performance moyenne du panier.
         */
        double compute_perf_moyenne_panier(const PnlMat *path) const;
        /**
         * @brief Calcule les valeurs moyennes des indices à une année N.
         * 
         * @param valeurs[out] Vecteur où stocker les valeurs calculées.
         * @param path Path contenant tous les prix.
         * @param N Année N à calculer.
         */
        void compute_valeurs_n_ans(PnlVect *valeurs, const PnlMat *path, int N) const;
        /**
         * @brief Calcule les performances moyennes à une année N.
         * 
         * @param perfs[out] Vecteur où stocker les performances calculées.
         * @param path Path contenant tous les prix.
         * @param N Année N à calculer.
         */
        void compute_perfs_n_ans(PnlVect *perfs, const PnlMat *path, int N) const;
        /**
         * @brief Détermine s'il y a un effet nouveau départ ou pas. Stock dans l'attribut nouveau_depart_.
         * 
         * @param path Path contenant tous les prix.
         */
        void compute_nouveau_depart(const PnlMat *path); 
        /**
         * @brief Calcule du flux du produit à l'année N. Correspond au C(N) déterminé dans le paragraphe 1.3 de l'analyse financière.
         * 
         * @param path Path contenant tous les prix.
         * @param N Année N à calculer.
         * @return double Flux du produit.
         */
        double compute_flux_n_ans(const PnlMat *path, int N) const;
        double payoff(const PnlMat *path);
};
