#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"

/**
 * @brief Calcule une matrice de log returns d'une matrice de prix. 
 * Possible d'indiquer start et end pour indiquer les indices entre lesquelles on souhaite calculer si on ne veut pas avoir tous les log returns.
 * start et end sont inclus.
 * 
 * @param log_returns[out] Les log returns calculés.
 * @param path Matrice de prix.
 * @param start Indice à laquelle il faut commencer à calculer les log_returns.
 * @param end Indice à laquelle arrêter.
 */
void log_returns(PnlMat * const log_returns, const PnlMat * const path, int start, int end);
/**
 * @brief Calcule les moyennes d'une matrice de prix.
 * 
 * @param means[out] Les moyennes calculées.
 * @param path Matrice de prix.
 */
void means(PnlVect * const means, const PnlMat * const path);
/**
 * @brief Calcule la covariance entre deux produits dans une matrice de prix.
 * 
 * @param log_returns Log returns des prix.
 * @param means Moyenne des prix.
 * @param k Indice du produit 1 qu'on souhaite calculer la covariance.
 * @param l Indice du produit 2 qu'on souhaite calculer la covariance.
 * @return double Covariance calculée.
 */
double compute_covariance(const PnlMat * const log_returns, const PnlVect * const means, int k, int l); // compute covariance between index k and l
/**
 * @brief Calcule la matrice de covariances d'une matrice de prix.
 * Possible d'indiquer start et end pour indiquer les indices entre lesquelles on souhaite calculer si on ne veut pas prendre toutes les données.
 * start et end sont inclus.
 * 
 * @param covariances[out] Matrice de covariances calculée.
 * @param path Matrice de prix.
 * @param start Indice à laquelle il faut commencer à prendre les données.
 * @param end Indice à laquelle il faut arrêter.
 */
void compute_covariances(PnlMat * const covariances, const PnlMat * const path, int start, int end); // compute covariance entre deux dates
/**
 * @brief Calcule la matrice sigma d'une matrice de prix.
 * Possible d'indiquer start et end pour indiquer les indices entre lesquelles on souhaite calculer si on ne veut pas prendre toutes les données.
 * start et end sont inclus.
 *
 * @param sigma[out] Matrice sigma calculée.
 * @param path Matrice de prix.
 * @param start Indice à laquelle il faut commencer à prendre les données.
 * @param end Indice à laquelle il faut arrêter.
 */
void compute_sigma(PnlMat * const sigma, const PnlMat * const path, int start, int end);
/**
 * @brief La volatilité correspond aux valeurs diagonales de la matrice sigma.
 * 
 * @param volatility[out] Volatilités.
 * @param sigma Matrice sigma.
 */
void compute_volatility(PnlVect * const volatility, const PnlMat * const sigma);