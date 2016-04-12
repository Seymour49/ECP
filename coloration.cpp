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

#include "coloration.h"
#include <random>
#include <chrono>
using namespace std;


Coloration::Coloration(Graphe* graphe, int k):G(graphe), nbColor(k){

}

Coloration::Coloration(const Coloration& other):G(other.G), nbColor(other.nbColor)
{

}

Coloration::~Coloration()
{
  delete(G);
}

ostream& Coloration::print(ostream& out)
{
  out << *G ;
  out << "Coloration" << endl;
  out << nbColor << endl;
  
  return out;
  
}

void Coloration::updateMafterInsert(int vertex, int color){
  
  for(int k = 0; k < G->getNbVertices(); ++k){
      M[k][color] += G->getMatriceValue(vertex,k);
  }

}

// return min = sommet non assigné (€ U) qui à le moins de voisins dans Vk[i]
int Coloration::selectVertex(vector<int> U,int color){
  
  int min = 0;
  int j = 1;
  
  bool found = false;
  
  while( (found != true && j < G->getNbVertices()) ){
    
    if( find(U.begin(), U.end(), j) != U.end() ){
      
      if( M[min][color] == 0 ){
	found = true; 
      }
      else if( M[min][color] > M[j][color] ){
	min = j;
      }
      
    }
    ++j;
  }
  
  return min;
}


void Coloration::initialisation()
{
  int i;
  
  for(i=0; i < nbColor; ++i){
      Vk[i].clear();
  }
  
  vector<int> U;
  // initialisation U = V
  // initialisation de la structure de donnée M
  M.resize(G->getNbVertices());
  for(i=0; i < G->getNbVertices() ; ++i){
    vector<int> v;
    v.resize(nbColor);
    for(int k = 0; k < nbColor; ++k){
      v.push_back(0);
    }
    
    M.push_back(v);    
    U.push_back(i);    
  }
  
  // On mélange U afin de sélectionner les k premiers aléatoirement
  size_t seed = chrono::system_clock::now().time_since_epoch().count();
  shuffle(U.begin(),U.end(),default_random_engine(seed));
  
  for(i=0 ; i < nbColor; ++i){
    int s = U.back();
    U.pop_back();
    Vk[i].push_back(s); 
    updateMafterInsert(s,i);	// f(vertex,color)
  }
  
  i = 0;
  
  while(!U.empty()){
    int v = selectVertex(U,i); // f(vector<int>U, color i)
    
    Vk[i].push_back(v);
    i = 1 + (i & (nbColor-1));
  }
  
  
}
