/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef ITERATEDTABUSEARCH_H
#define ITERATEDTABUSEARCH_H

#include "basictabusearch.h"
#include "onemove.h"
#include "swap.h"
#include <random>
#include <chrono>
#include <exception>

class IteratedTabuSearch
{
public:
    IteratedTabuSearch(Graphe* graphe, int b , int k, int d,double rt);
    IteratedTabuSearch(const IteratedTabuSearch& other);
    virtual ~IteratedTabuSearch();
    
    Coloration* run();
    
    /**
     * Fonction déterminant si la perturbation sera aléatoire ou dirigée.
     */
    void perturbate(Coloration* prime);
    
    /*
     * Perturbation aléatoire
     */
    void randomPertubation(Coloration* prime);
    
    /*
     * Perturbation dirigée
     */
    void directedPerturbation(Coloration* prime);
    
    /** Fonction initialisant le voisinage de la solution passée
     * en paramètre dans le vecteur également passé en paramètre
     * @param current Coloration dont on souhaite calculer le voisinage
     * @param Neighboor Vecteur de voisins allant contenir le résultat
     */
    void initNeighboor(Coloration* current, std::vector<Voisin *> *Neighboor);
    
    /** Fonction calculant le gain associé à l'ensemble des voisins
     * du vecteur passé en paramètre
     * @param current Coloration source du calcul
     * @param Neighboor vecteur de voisins dont on souhaite calculer le gain
     */
    void calculDelta(Coloration* current, std::vector< Voisin* > Neighboor);
    
    /** Fonction booléenne appelant la sous-fonctions
     * correspondant au type de voisin passé en paramètre
     * @param Neighboor voisin à tester, voisin*
     * @param iteration iteration courante, entier
     * @param tabuMat matrice tabu sur laquelle effectuer la vérification, vector<vector<int>>
     */
    bool isForbidden(Voisin* Neighboor, int iter, std::vector<std::vector<int> >&tabuMat);
    
    /** Fonction booléenne vérifiant la validité d'un voisin OneMove
     * @param om voisin à tester, OneMove*
     * @param iteration iteration courante, entier
     * @param tabuMat matrice tabu sur laquelle effectuer la vérification, vector<vector<int>>
     */
    bool isForbiddenOM(OneMove* om, int iter, std::vector< std::vector< int > >& tabuMat);
    
    /** Fonction booléenne vérifiant la validité d'un voisin OneMove
     * @param s voisin à tester, Swap*
     * @param iteration iteration courante, entier
     * @param tabuMat matrice tabu sur laquelle effectuer la vérification, vector<vector<int>>
     */
    bool isForbiddenS(Swap* s, int iter, std::vector<std::vector<int> >&tabuMat);
        

private:
    Graphe* G;
    int beta;
    int nbColor;
    int depth;
    // TODO Ajouter randomProba en paramètre de classe
    Coloration s;
    
    // Gestion de la timeLine
    double remainingTime;
};

#endif // ITERATEDTABUSEARCH_H
