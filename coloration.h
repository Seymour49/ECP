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

#ifndef COLORATION_H
#define COLORATION_H

#include "graphe.h"

class Coloration
{
public:
  Coloration(Graphe* graphe, int k);
  Coloration(const Coloration& other);
  ~Coloration();
  //Coloration& operator=(const Coloration& other);
  
  int getNbColor() const { return nbColor; }
  
  std::ostream& print(std::ostream& out);      
  friend std::ostream& operator<<(std::ostream& out, Coloration& r){
      return r.print(out);
  }
  /**
   * Procédure de mise à jour de M après insertion d'un sommet vertex dans une partition color
   */
  void updateMafterInsert(int vertex, int color);
  
  /**
   * Fonction de sélection d'un sommet j à ajouter à la partition color.
   */
  int selectVertex(std::vector< int > U, int color);
  /**
   * Méthode d'initialisation respectant la contrainte d'équité des capacités.
   */
  void initialisation();
  
  /**
   * Fonction retournant le nombre de sommets en conflits
   */
  int evaluate();
private:
    Graphe* G;
    int nbColor;
    std::vector<std::vector<int> > Vk; /* Vk[i][j] = indice dans V du j_ième sommet de couleur i */
    std::vector<std::vector<int> > M; /* M[i][j] = nombre de voisins du sommet i de couleur j */
};

#endif // COLORATION_H
