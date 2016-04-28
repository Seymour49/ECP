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
    /** Constructeur et destructeur */
    BasicTabuSearch(Coloration* init, int prof);
    ~BasicTabuSearch();
    
    /**
     * Méthode principale retournant la meilleure coloration trouvée
     * par cette recherche tabou
     */
    Coloration* run();
    
    /**
     * Fonction d'initalisation de la matrice tabou ( tout à 0 )
     */
    void initTabuMat();
    
    /**
     * Fonction calculant l'ensembles des voisins admissibles par la solution
     * courante.
     */
    void initNeighboor();
    
    /**
     * Fonction calculant le gain associé à l'ensemble des voisins admissibles
     */
    void calculDelta();
    
    
    /**
     * Procédures de contrôle afin de vérifier qu'un mouvement est interdit
     */
    bool isForbidden( Voisin* N, int iter);
    bool isForbiddenOM( OneMove* om, int iter);
    bool isForbiddenS( Swap *s, int iter);
    
private:
    Coloration &current;
    int tabuTenure;
    int depth;
     std::vector<Voisin *> N;
     std::vector< std::vector<int> > tabuMat;
};

#endif // BASICTABUSEARCH_H
