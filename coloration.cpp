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
//   out << *G ;
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
  
  out << "Sommets en conflit :";
  for(i=0; i < Vk.size(); ++i){
      for(j = 0; j < Vk[i].size(); ++j){
	  if(inConflict(i,j))
	    out << " " << Vk[i][j];
      }
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
    vector< vector<int> >M_ = M;
    vector< vector<int> > Vk_ = Vk;
    
    // Application du voisin
    for(int i=0; i < G->getNbVertices(); ++i){
	// Les sommets s et i sont voisins
	if( G->getMatriceValue(om->getS(), i) == 1){
	    M_[i][om->getVki()] -= 1;
	    M_[i][om->getVkj()] += 1;
	}
    }
    
    // Mise à jour de Vk_
    Vk_[om->getVki()].erase( remove(Vk_[om->getVki()].begin(), Vk_[om->getVki()].end(), om->getS()), Vk_[om->getVki()].end() );
    Vk_[om->getVkj()].push_back(om->getS());
    
    // Evaluation de Vk_ vis à vis de M_
    int result = 0;
    
    for(unsigned i=0; i < Vk_.size(); ++i){
    
	for(unsigned j=0; j < Vk_[i].size(); ++j){
	
	    result += (int)(M_[Vk_[i][j]][i]  != 0);
	}
    }
    
    return result;
}

int Coloration::simulEvalS(Swap* s){
    vector< vector<int> >M_ = M;
    vector< vector<int> > Vk_ = Vk;
    
    
    // Application du voisin --> correspond à deux OneMove consécutifs
    for(int i=0; i < G->getNbVertices(); ++i){
	// Les sommets s et i sont voisins
	if( G->getMatriceValue(s->getSi(), i) == 1){
	    M_[i][s->getKi()] -= 1;
	    M_[i][s->getKj()] += 1;
	}
	
	if(G->getMatriceValue(s->getSj(),i) == 1){
	    M_[i][s->getKj()] -= 1;
	    M_[i][s->getKi()] += 1;
	}
    }
    
    // Mise à jour de Vk_
    Vk_[s->getKi()].erase( remove(Vk_[s->getKi()].begin(), Vk_[s->getKi()].end(), s->getSi()), Vk_[s->getKi()].end() );
    Vk_[s->getKj()].push_back(s->getSi());
    
    Vk_[s->getKj()].erase( remove(Vk_[s->getKj()].begin(), Vk_[s->getKj()].end(), s->getSj()), Vk_[s->getKj()].end() );
    Vk_[s->getKi()].push_back(s->getSj());
    
    // Evaluation de Vk_ vis à de M_
    int result = 0;
    
    for(unsigned i=0; i < Vk_.size(); ++i){
    
	for(unsigned j=0; j < Vk_[i].size(); ++j){
	
	    result += (int)(M_[Vk_[i][j]][i]  != 0);
	}
    }
    
    return result;
}

void Coloration::validOneMove(OneMove* om){
    
    // MàJ de M
    for(int i = 0; i < G->getNbVertices(); ++i){
	
	if(G->getMatriceValue(om->getS(),i) == 1){
	    M[i][om->getVki()] -= 1;
	    M[i][om->getVkj()] += 1;
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
	    M[i][s->getKi()] -= 1;
	    M[i][s->getKj()] += 1;
	}
	
	if(G->getMatriceValue(s->getSj(),i) == 1){
	    M[i][s->getKj()] -= 1;
	    M[i][s->getKi()] += 1;
	}
    }
    
    // Mise à jour de Vk_
    Vk[s->getKi()].erase( remove(Vk[s->getKi()].begin(), Vk[s->getKi()].end(), s->getSi()), Vk[s->getKi()].end() );
    Vk[s->getKj()].push_back(s->getSi());
    
    Vk[s->getKj()].erase( remove(Vk[s->getKj()].begin(), Vk[s->getKj()].end(), s->getSj()), Vk[s->getKj()].end() );
    Vk[s->getKi()].push_back(s->getSj());
}

