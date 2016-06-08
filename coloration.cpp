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


Coloration::Coloration(Graphe* graphe):G(graphe){

}

Coloration::Coloration(const Coloration& other){

    G = other.G;
    nbColor = other.nbColor;
    Vk = other.Vk; 
    M = other.M;
}

Coloration::~Coloration()
{
}

Coloration& Coloration::operator=(const Coloration& other)
{
    if( &other != this){
	G = other.G;
	nbColor = other.nbColor;
	Vk = other.Vk; 
	M = other.M;
    }
    return *this;
}

ostream& Coloration::print(ostream& out)
{
  unsigned i,j;
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

void Coloration::clearVector(){
  if(!(Vk.empty()) ){
	Vk.clear();
  }
  if(!(M.empty())){
      M.clear();
  }
}

void Coloration::initialisation(int nbc)
{
  int i;
  nbColor = nbc;
  
  clearVector();
  
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
      
      total += (int)(inConflict(i,j));
    }
  }
    
  return total;
  
}

bool Coloration::inConflict(int i, int j){
    return ( M[Vk[i][j]][i]  != 0 ) ;
}

int Coloration::calculDeltaOM(OneMove* om){
    
    int result = M[om->getS()][om->getVkj()] - M[om->getS()][om->getVki()];
    return result;
}

int Coloration::calculDeltaS(Swap* s){
    
    int result = M[s->getSj()][s->getKi()] - M[s->getSj()][s->getKj()];
    result += (M[s->getSi()][s->getKj()] - M[s->getSi()][s->getKi()] - 2*G->getMatriceValue(s->getSi(),s->getSj()) );
    return result;
}

int Coloration::simulEvalOM(OneMove* om){
    
    int result = 0;
    for(int i=0; i < nbColor; ++i){
	for(unsigned j=0; j < Vk[i].size(); ++j){
	    
	    if( (G->getMatriceValue(Vk[i][j],om->getS()) == 1) && (i == om->getVkj()) ){
		// Pas besoin de test car on ferait (M[.][.] + 1) != 0 --> Tautologie 
		++result;
	    }
	    else if( (G->getMatriceValue(Vk[i][j],om->getS()) == 1) && (i == om->getVki()) ){
		result += ( (M[Vk[i][j]][i]-1) != 0 ); 
	    }
	    else{
		result += ( M[Vk[i][j]][i] != 0);
	    }   
	}
    }
    return result;
}

int Coloration::simulEvalS(Swap* s){
    int result = 0;
    
    for(int i=0; i< nbColor; ++i){
	for(unsigned j=0; j < Vk[i].size(); ++j){
	    
	    if( (G->getMatriceValue(Vk[i][j],s->getSi()) == 1) && (i == s->getKj()) ){
		// Pas besoin de tester M !=0
		++result;
	    }else if( (G->getMatriceValue(Vk[i][j],s->getSi()) == 1) && (i == s->getKi()) ){
		result += (int)( (M[Vk[i][j]][i]-1) != 0);
	    }
	    else{
		result += (int)( M[Vk[i][j]][i] != 0 );
	    }
	    
	    if( (G->getMatriceValue(Vk[i][j],s->getSj()) == 1) && ( i == s->getKi()) ){
		++result;
	    }else if( (G->getMatriceValue(Vk[i][j],s->getSj()) == 1) && ( i == s->getKj()) ){
		result += (int)( (M[Vk[i][j]][i] -1) != 0 );
	    }
	    else{
		result += (int)( M[Vk[i][j]][i] != 0);
	    }
	}
    }
     return result;
}

void Coloration::validOneMove(OneMove* om){
    
    // MàJ de M
    for(int i = 0; i < G->getNbVertices(); ++i){
	
	if(G->getMatriceValue(om->getS(),i) == 1){
	    M[i][om->getVkj()] += 1;
	    M[i][om->getVki()] -= 1;
	}
    }
    
    // MàJ de Vk
    Vk[om->getVki()].erase( remove(Vk[om->getVki()].begin(), Vk[om->getVki()].end(), om->getS()), Vk[om->getVki()].end() );
    Vk[om->getVkj()].push_back(om->getS());
}


void Coloration::validSwap(Swap* s){
    // MàJ de M --> correspond à deux OneMove consécutifs
    for(int i=0; i < G->getNbVertices(); ++i){
	// Les sommets s et i sont voisins
	if( G->getMatriceValue(s->getSi(), i) == 1){
	    M[i][s->getKj()] += 1;
	    M[i][s->getKi()] -= 1;
	}
	
	if(G->getMatriceValue(s->getSj(),i) == 1){
	    M[i][s->getKi()] += 1;
	    M[i][s->getKj()] -= 1;
	}
    }
    
    // Mise à jour de Vk
    Vk[s->getKi()].erase( remove(Vk[s->getKi()].begin(), Vk[s->getKi()].end(), s->getSi()), Vk[s->getKi()].end() );
    Vk[s->getKj()].push_back(s->getSi());
    
    Vk[s->getKj()].erase( remove(Vk[s->getKj()].begin(), Vk[s->getKj()].end(), s->getSj()), Vk[s->getKj()].end() );
    Vk[s->getKi()].push_back(s->getSj());
}

bool Coloration::areAdjacent(int dep, int arr){
    return ( G->getMatriceValue(dep,arr) == 1);
}

void Coloration::decreaseM(int i, int ki){
    M[i][ki] -= 1;
}

void Coloration::increaseM(int i, int ki){
    M[i][ki] += 1;
}

void Coloration::moveM(int si, int ki, int kj){
    Vk[ki].erase( remove(Vk[ki].begin(), Vk[ki].end(), si), Vk[ki].end() );
    Vk[kj].push_back(si);
}
