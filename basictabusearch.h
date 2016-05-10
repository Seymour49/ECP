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

#ifndef BASICTABUSEARCH_H
#define BASICTABUSEARCH_H

#include "coloration.h"

/**
 * Cette classe représente la procédure de recherche Tabou initiale
 * présentée au point 2.4.2 de l'article lié.
 * Le coeur de cette classe est le méthode run() éxecutant la recherche
 * selon les paramètres de l'instance et retournant un pointeur sur la 
 * meilleure coloration trouvée.
 * 
 */


class BasicTabuSearch
{
public:
    /** Constructeur unique
     * @param init Coloration initiale de la recherche tabou
     * @param prof profondeur de la recherche tabou 
     */
    BasicTabuSearch(Coloration* init, int prof);
    
    /** Destructeur
     */
    ~BasicTabuSearch();
    
    /** Méthode principale de la recherche tabou basique
     * Respecte l'algorithme décrit au point 2.4.2 de l'article lié.
     * @return meilleur coloration trouvée. 
     */
    Coloration* run();
    
    /** Fonction d'initalisation de la matrice tabou.
     */
    void initTabuMat();
    
    /** Fonction calculant l'ensemble des voisins OneMove admissibles par la solution
     * courante.
     */
    void initNeighboorOM();
    
    /** Fonction calculant l'ensemble des voisins Swap admissibles par la solution
     * courante.
     */
    void initNeighboorS();    
    
    /** Fonction calculant le gain associé à l'ensemble des voisins admissibles
     */
    void calculDelta();
    
    
    /** Procédure de vérification qu'un mouvement de voisinage n'est pas interdit
     * Effectue un dynamic_cast et appelle la fonction correspondante
     * @param N Voisin à caster et tester
     * @param iter iteration courante
     * @return resultat de la sous-fonction appelée
     */
    bool isForbidden( Voisin* N, int iter);
    
    /** Procédure de vérification qu'un OneMove ne soit pas interdit
     * @param om OneMove à tester
     * @param iter iteration courante de la recherche
     * @return ( tabuMat[om->getS()][om->getVkj()] > iter )
     */
    bool isForbiddenOM( OneMove* om, int iter);
    
    /** Procédure de vérification qu'un Swap ne soit pas interdit
     * @param s Swap à tester
     * @param iter iteration courante de la recherche
     * @return ( (tabuMat[s->getSi()][s->getKj()] > iter) || (tabuMat[s->getSj()][s->getKi()] > iter) )
     */
    bool isForbiddenS( Swap *s, int iter);
    
private:
    Coloration &current;
    int tabuTenure;
    int depth;
     std::vector<Voisin *> N;
     std::vector< std::vector<int> > tabuMat;
};

#endif // BASICTABUSEARCH_H
