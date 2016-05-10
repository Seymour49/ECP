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

#ifndef SWAP_H
#define SWAP_H

#include "voisin.h"

/**
 * Cette classe implément le second voisinage décrit dans l'article
 * lié à ce programme. Sa signature est la suivante :
 * Swap s(i,j,k,l) où i est coloré en k, j est coloré en l et le 
 * mouvement correspond à déplacer i vers la couleur l et le
 * sommet j vers la couleur k.
 * 
 */

class Swap : public Voisin
{
public:
    /** Constructeur 
     @param i sommet Si
     @param j sommet Sj
     @param k couleur du sommet Si
     @param l couleur du sommet Sj
    */
    Swap(int i, int j, int k, int l);
    
    /** Getter sur Si
     */
    int getSi() const { return si; }

    /** Getter sur Sj
     */
    int getSj() const { return sj; }
    
    /** Getter sur Ki
     */
    int getKi() const { return ki; }
    
    /** Getter sur Kj
     */
    int getKj() const { return kj; }
    
    /** Display */
    std::ostream& print(std::ostream& out);
    
private:
    int si;
    int sj;
    int ki;
    int kj;
};

#endif // SWAP_H
