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
  unsigned i,j;
  out << *G ;
  out << "Coloration" << endl;
  out << " Partitions  : " << endl;
  for(i = 0; i < Vk.size(); ++i){
    out << "Vk[" << i << "] :";
    for(j = 0; j < Vk[i].size(); ++j){
	out << " " << Vk[i][j] ;
    }
    out << " -->  " << Vk[i].size() << " éléments." << endl;
    
  }  
  
  out << "Matrice M" << endl;
  for(i = 0 ; i < M.size(); ++i){
	out << "M[" <<i<<"] :" ;
	for(j=0; j < M[i].size(); ++j){
	  out << " " << M[i][j];
	}
	out << endl;
  }
  out << endl;
  return out;
  
}

void Coloration::updateMafterInsert(int vertex, int color){
  
  for(int k = 0; k < G->getNbVertices(); ++k){
      M[k][color] += G->getMatriceValue(vertex,k);
  }

}

// return min = sommet non assigné (€ U) qui à le moins de voisins dans Vk[i]
int Coloration::selectVertex(vector<int> U,int color){
  
  int min = U.at(0);
  
  for(unsigned j=1; j < U.size(); ++j){
      if(M[min][color] == 0 ){
	return min;
      }
      else if(M[min][color] > M[U.at(j)][color] ){
	min = U.at(j);
      }
  }
  
  return min;
}


void Coloration::initialisation()
{
  int i;
  
  for(i=0; i < nbColor; ++i){
      vector<int> k;
      Vk.push_back(k);
  }
  
  vector<int> U;
  // initialisation U = V
  // initialisation de la structure de donnée M
  for(i=0; i < G->getNbVertices() ; ++i){
    vector<int> v;
    //v.resize(nbColor);
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
    
    updateMafterInsert(v, i);	// f(vertex,color)
    
    U.erase( remove(U.begin(),U.end(), v), U.end());
    i = (i+1)  % nbColor;
  }
  
}

int Coloration::evaluate(){

  int total = 0;
  
  for(unsigned i=0; i < Vk.size(); ++i){
    
    for(unsigned j=0; j < Vk[i].size(); ++j){
      
      total += (int)(M[Vk[i][j]][i] != 0);
    }
  }
    
  return total;
  
}

