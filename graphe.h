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



class Graphe
{
private:
  std::string namefile;
  int nbVertices;
  int nbEdges;
  std::vector< std::vector<bool> >grapheMatrice;
  
public:
    
  Graphe();
  Graphe(std::string s);
  Graphe(const Graphe& other);
  ~Graphe();
  Graphe& operator=(const Graphe& other);
  
  std::string getNamefile() const { return namefile; }
  int getNbVertices() const { return nbVertices; }
  int getMatriceValue(int x, int y) const { return (int)(grapheMatrice[x][y]); }
  
  std::ostream& print(std::ostream& out);      
  friend std::ostream& operator<<(std::ostream& out, Graphe& r){
      return r.print(out);
  }
  
  void initMatrice();
  bool tryLoadFile();

  
};

#endif // GRAPHE_H
