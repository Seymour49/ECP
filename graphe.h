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

#ifndef GRAPHE_H
#define GRAPHE_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/iterator/iterator_concepts.hpp>
#include <cstdlib>
#include <cmath>

/**
 * Cette classe représente un graphe dont le fichier source 
 * (format DIMACS) est passé en paramètre au constructeur.
 * Une matrice G de booléens taille N*N (où N est le nombre de 
 * sommets est créée pour représenter ce graphe.
 * Ainsi G[u][v] = 1 ssi (u,v) € E (ensemble d'arcs)
 * 		   0 sinon.
 * 
 */

class Graphe
{
  
public:
  /** Constructeur par défaut.
   * @param s nom du fichier source du graphe au format DIMACS
   */
  Graphe(std::string s);
  
  /** Constructeur par recopie
   * @param other Graphe à recopier
   */
  Graphe(const Graphe& other);
  
  /** Destructeur
   */
  virtual ~Graphe();
  
  /** Opérateur =
   * @param other Graphe à droite de l'égalité
   */
  Graphe& operator=(const Graphe& other);
  
  /** Getter sur le nom de fichier 
   * @return nameFile
   */
  std::string getNamefile() const { return namefile; }
  
  /** Getter sur le nombre de sommets du graphe
   * @return nbVertices
   */
  int getNbVertices() const { return nbVertices; }
  
  /** Getter sur la présence d'un arc entre deux sommets
   * @param x sommet de départ
   * @param y sommet d'arrivée
   * @return grapheMatrice[x][y] = 1 si arc, 0 sinon
   */
  int getMatriceValue(int x, int y) const { return (int)(grapheMatrice[x][y]); }
  
  /* Display */
  std::ostream& print(std::ostream& out);      
  friend std::ostream& operator<<(std::ostream& out, Graphe& r){
      return r.print(out);
  }
  
  /** Fonction initialisant grapheMatrice selon le nombre
   * de sommets du graphe.
   */
  void initMatrice();
  
  /** Fonction de chargement du fichier source.
   * @return true si le chargement s'est bien passé, false sinon
   */
  bool tryLoadFile();

private:
  std::string namefile;
  int nbVertices;
  int nbEdges;
  std::vector< std::vector<bool> >grapheMatrice;
  
};

#endif // GRAPHE_H
