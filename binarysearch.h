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

#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include "basictabusearch.h"

/**
 * Cette classe représente la recherche binaire permettant dans le papier 
 * étudié de déterminer un nombre de couleur initial interessant pour ensuite
 * effectuer une recherche tabou itérée.
 * 
 * Pour plus de détails, veuillez-vous reportez à la section 2.3 de l'article
 * de référence :
 * 
 * http://www.info.univ-angers.fr/pub/hao/papers/LaiHaoGloverEAAI2015.pdf * 
 * 
 */

class BinarySearch
{
public:
    /** Constructeur par défaut.
     * @param graphe Graphe concerné par l'étude de l'ECP en cours
     * @param d profondeur de la BasicTabuSearch utilisée au cours de l'execution
     */
    BinarySearch(Graphe* graphe, int d);
    
    /** Constructeur par recopie
     * @param other référence sur la copie servant d'initialiseur
     */
    BinarySearch(const BinarySearch& other);
    
    /** Destructeur 
     */
    virtual ~BinarySearch();
    
    /** Méthode principale de la recherche binaire dont l'algorithme est
     * présentée dans l'article source.
     * @return la meilleur k-coloration initiale
     */
    Coloration* run();

private:
    Graphe* G;
    int depth;
    Coloration s;
};

#endif // BINARYSEARCH_H
