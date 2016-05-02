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

#include "onemove.h"
#include "swap.h"
#include "graphe.h"
#include <cassert>
#include <exception>

/**
 * Classe représentant une coloration équitable. Afin d'y parvenir
 * nous utilisons un vecteur de vecteurs de sommets représentant 
 * chacun un ensemble de sommets colorés de la même couleur.
 * Afin d'évaluer les opérations de voisinages, cette classe
 * dispose également d'une matrice M de taille N*k (où N est le 
 * nombre de sommets et k le nombre de couleur de la coloration)
 * représentant le nombre de sommets adjacents au sommet u de couleur
 * j (0<j<k).
 * 
 */

class Coloration
{
public:
  /** Constructeurs et destructeur */
  Coloration(Graphe* graphe);
  Coloration(const Coloration& other);
  virtual ~Coloration();
  
  /** Opérateur = */
  Coloration& operator=(const Coloration& other);
  
  /** Getters */
  int getNbColor() const { return nbColor; }
  int getNbVertices() const { return G->getNbVertices(); }
  int getVkiSize(int i){ return Vk[i].size(); }
  int getValueVk(int i, int j){ assert((unsigned) i< Vk.size()); assert((unsigned) j < Vk[i].size()); return Vk[i][j]; }
  
  /** Display */
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
  void initialisation(int nbc);
  
  /**
   * Fonction retournant le nombre de sommets en conflits
   */
  int evaluate();
  
  /** Fonction retournant vrai si le sommet j de couleur i testé est en conflit, i.e 
   * M[Vk[i][j]][i] != 0
   */
  bool inConflict(int i, int j);
  
  /**
   * Fonction simulant l'évaluation sur la coloration courante à laquelle
   * on applique le voisin OneMove passé en paramètre.
   * Utilisée pour bruiter le caractère tabou d'un voisin
   */
  int simulEvalOM(OneMove* om);
  
  /**
   * Fonction simulant l'évaluation sur la coloration courante à laquelle
   * on applique le voisin Swap passé en paramètre.
   * Utilisée pour bruiter le caractère tabou d'un voisin
   */
  int simulEvalS(Swap* s);
  
  /**
   * Fonction calculant le delta d'un voisin OneMove
   */
  int calculDeltaOM(OneMove* om);
  
  /**
   * Fonction calcultant le delta d'un voisin Swap
   */
  int calculDeltaS(Swap * s);
  
  /**
   * Procédures de validation d'un mouvement voisin passé en paramètre
   */  
  void validOneMove(OneMove* om);
  void validSwap(Swap* s);
  
  /**
   * Procédure de nettoyage des vecteurs Vk et M
   */
  void clearVector();
private:
    Graphe* G;
    int nbColor;
    std::vector<std::vector<int> > Vk; /* Vk[i][j] = indice dans V du j_ième sommet de couleur i */
    std::vector<std::vector<int> > M; /* M[i][j] = nombre de voisins du sommet i de couleur j */
        
};

#endif // COLORATION_H
