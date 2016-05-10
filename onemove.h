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

#ifndef ONEMOVE_H
#define ONEMOVE_H

#include "voisin.h"

/**
 * Cette classe représente le premier voisinage décrit dans l'article
 * lié à ce programme. Sa signature est la suivante :
 * OneMove om(s,i,j) où s est le sommet à déplacer de sa couleur courante
 * i vers la couleur j.
 * 
 */

class OneMove : public Voisin
{
public:
    /** Constructeur unique
     * @param vertex sommet concerné
     * @param ci couleur source
     * @param cj couleur destination
     */
    OneMove(int vertex, int ci, int cj);
    
    /** Getter sur le sommet
     */
    int getS() const { return s; }
    
    /** Getter sur la couleur d'origine
     */
    int getVki() const { return vki; }
    
    /** Getter sur la couleur d'arrivée
     */
    int getVkj() const { return vkj; }

    /** Display */
    std::ostream& print(std::ostream& out);
    
private:
    int s;
    int vki;
    int vkj;
};

#endif // ONEMOVE_H
