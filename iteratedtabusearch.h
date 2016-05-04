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

class IteratedTabuSearch
{
public:
    IteratedTabuSearch(Graphe* graphe, int b , int k, int d);
    IteratedTabuSearch(const IteratedTabuSearch& other);
    virtual ~IteratedTabuSearch();
    
    Coloration* run();
    
    /**
     * Fonction déterminant si la perturbation sera aléatoire ou dirigée.
     */
    Coloration* perturbate(Coloration* current);
    
    /*
     * Perturbation aléatoire
     */
    Coloration* randomPertubation(Coloration* current);
    
    /*
     * Perturbation dirigée
     */
    Coloration* directedPerturbation(Coloration* current);
private:
    Graphe* G;
    int beta;
    int nbColor;
    int depth;
    // TODO Ajouter randomProba en paramètre de classe
    Coloration s;
};

#endif // ITERATEDTABUSEARCH_H
