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
  /** Constructeur par défault.
   * @param graphe Graphe allant être étudié
   */
  Coloration(Graphe* graphe);
  
  /** Constructeur par recopie.
   * @param other Coloration source à copier   * 
   */
  Coloration(const Coloration& other);
  
  /** Destructeur 
   */
  virtual ~Coloration();
  
  /** Opérateur =.
   * @param other Coloration droite de l'égalité.
   */
  Coloration& operator=(const Coloration& other);
  
  
  /** Getter sur nbColor
   *  @return nombre de couleur de la coloration, entier	
   */
  
  int getNbColor() const { return nbColor; }
  /** Getter sur NbVertices. Fait appel à Graphe::getNbVertices
   * @return nombre de sommets du graphe, entier
   */
  int getNbVertices() const { return G->getNbVertices(); }
  
  /** Getter sur Vk[i].size()
   * @param i couleur dont on souhaite connaître le nombre d'éléments
   * @return nombre d'élements colorés de la couleur passé en paramètre, entier
   */
  int getVkiSize(int i){ return Vk[i].size(); }
  
  /** Getter sur Vk[i][j]
   * @param i couleur du sommet à retourner
   * @param j indice dans Vk[i] du sommet à retourner
   * @return identifiant du sommet d'indice j dans la couleur i
   */
  int getValueVk(int i, int j){ assert((unsigned) i< Vk.size()); assert((unsigned) j < Vk[i].size()); return Vk[i][j]; }
  
  /** Display */
  std::ostream& print(std::ostream& out);      
  friend std::ostream& operator<<(std::ostream& out, Coloration& r){
      return r.print(out);
  }
  
  /** Procédure de mise à jour de M après insertion d'un sommet dans une partition
   * @param vertex identifiant du sommet récemment coloré
   * @param color couleur utilisé pour colorer vertex
   */
  void updateMafterInsert(int vertex, int color);
  
  /** Fonction de sélection d'un sommet j à ajouter à la partition color.
   * @param U ensemble de sommet modifié après en avoir sélectionné un.
   * @param color couleur dans laquelle colorer le sommet selectionné
   * @return sommet non assigné (€ U) qui à le moins de voisins dans Vk[color]
   */
  int selectVertex(std::vector< int > U, int color);
  
  /** Méthode d'initialisation respectant la contrainte d'équité des capacités.
   * @param nbc nombre de couleurs avec laquelle on souhaite initialiser la coloration
   * 
   */
  void initialisation(int nbc);
  
  /** Fonction d'évaluation retournant le nombre de sommets en conflits
   * @return nombre de sommets en conflits (i.e coloré de la même couleur
   * qu'au moins un de ses voisins.
   */
  int evaluate();
  
  /** Fonction retournant vrai si le sommet j de couleur i testé est en conflit 
   * @param i couleur du sommet à étudier
   * @param j indice dans Vk[i] du sommet à étudier
   * @return booléen = ( M[Vk[i][j]][i] != 0 )
   */
  bool inConflict(int i, int j);
  
  /** Fonction simulant l'évaluation sur la coloration courante à laquelle
   * on applique le voisin OneMove passé en paramètre.
   * Utilisée pour bruiter le caractère tabou d'un voisin
   * @param om OneMove dont on souhaite simuler l'impact
   * @return evaluation après simulation
   */
  int simulEvalOM(OneMove* om);
  
  /** Fonction simulant l'évaluation sur la coloration courante à laquelle
   * on applique le voisin Swap passé en paramètre.
   * Utilisée pour bruiter le caractère tabou d'un voisin
   * @param s Swap dont on souhaite simuler l'impact
   * @return evaluation après simulation, entier
   */
  int simulEvalS(Swap* s);
  
  /** Fonction calculant le delta d'un voisin OneMove
   * @param om OneMove dont on souhaite calculer le delta selon la coloration actuelle
   * @return M[s][Vk[j]] - M[s][Vk[i]], entier
   */
  int calculDeltaOM(OneMove* om);
  
  /** Fonction calcultant le delta d'un voisin Swap
   * @param s Swap dont on souhaite calculer le delta selon la coloration actuelle
   * @return M[v][K(u)] - M[v][K(v)] + (M[u][K(v)] - M[u][K(u)] - 2*G[u][v]), entier
   */
  int calculDeltaS(Swap * s);
  
  /** Procédure de validation d'un voisin OneMove
   * @param om OneMove à appliquer sur la coloration actuelle
   */  
  void validOneMove(OneMove* om);
  
  /** Procédure de validation d'un voisin Swap
   * @param s Swap à appliquer sur la coloration actuelle
   */
  void validSwap(Swap* s);
  
  /** Procédure de nettoyage des vecteurs Vk et M
   */
  void clearVector();
  
  /** Fonction booléenne testant si deux sommets sont voisins.
   * @param dep sommet départ
   * @param arr sommet arrivée
   * @return vrai si les sommets sont adjacents, faux sinon 
   */
  bool areAdjacent(int dep, int arr);
  
  /** Fonction ajoutant 1 à la case concernée dans M.
   * @param i sommet i concerné
   * @param ki couleur ki concernée
   */
  void increaseM(int i, int ki);
  
  /** Fonction retirant 1 à la case concernée dans M.
   * @param i sommet i concerné
   * @param ki couleur ki concernée
   */
  void decreaseM(int i, int ki);
 
  /** Fonction déplaçcant le sommet si de la couleur
   * ki vers la couleur kj
   * @param si identifiant du sommet à déplacer
   * @param ki couleur de départ
   * @param kj couleur d'arrivée
   */
  void moveM(int si, int ki, int kj);
  
private:
    Graphe* G;
    int nbColor;
    std::vector<std::vector<int> > Vk; /* Vk[i][j] = indice dans V du j_ième sommet de couleur i */
    std::vector<std::vector<int> > M; /* M[i][j] = nombre de voisins du sommet i de couleur j */
        
};

#endif // COLORATION_H
