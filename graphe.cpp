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

#include "graphe.h"
using namespace std;

/**
 * Fonction utile à la lecture du fichier source. Utilise le séparateur 
 * ' '
 * @param str : chaîne de caractères à découper
 * @return un pointeur sur un vecteur contenant chaque mot de str
 */
vector< string >& explode(const string& str){
  istringstream split(str);
  vector< string >* tokens = new vector<string>;

  for(string each; getline(split, each, ' '); tokens->push_back( each.c_str()) );

  return *tokens;
}


ostream& Graphe::print(ostream& out){
  out << "Graphe : " << namefile << endl;

  for(int i = 0; i < nbVertices ; ++ i){
    out << "Sommet " << i << " :";
    for(int j = 0; j < nbVertices ; ++j){
	out << " " <<grapheMatrice[i][j] ;
    }
    
    out << endl;	  
  }
  out << endl;
  return out;
}

void Graphe::initMatrice(){

  for(int i=0; i< nbVertices;++i){
      vector<bool> v;
      for(int j=0; j<nbVertices;++j){
	  v.push_back(false);
      }
      grapheMatrice.push_back(v);
  }
    
}
    

bool Graphe::tryLoadFile()
{
  ifstream f(namefile.c_str());
  int src, dest;
	
  if(!f){
      
      cerr << "Erreur pendant l'ouverture du fichier" << endl;
      return false;      
  }else{
      string line;
      
      while(getline(f,line)){
	  // La ligne est un commentaire, on l'ignore simplement
	  if(line.substr(0,1) == "c"){  
	    
	  }
	  // La ligne est une ligne de paramètre nous donnant le nombre de sommets et d'arcs
	  else if(line.substr(0,1) == "p"){
	      vector<string>& tokens = explode(line);
	      nbVertices = atoi(tokens[2].c_str());
	      
	      initMatrice();
	      delete(&tokens);
	  }else{
	      vector<string>& tokens = explode(line);
		/* tokens[0] : caractère de controle
		* tokens[1] : id du sommet source
		* tokens[2] : id du sommet destination
		*/
	      if(tokens[0] == "e"){
		  // Controle ok, on doit mettre à jour deux arcs (graphe non-orienté)
		  src = atoi(tokens[1].c_str());
		  dest = atoi(tokens[2].c_str());

		  (grapheMatrice.at(src-1)).at(dest-1) = 1;
		  (grapheMatrice.at(dest-1)).at(src-1) = 1;
		  ++nbEdges;
		  
	      }
	      delete(&tokens);
	  }
      }    
  }
  return true;

}

Graphe::Graphe(std::string s):namefile(s){
}

Graphe::Graphe(const Graphe& other):namefile(other.namefile), nbVertices(other.nbVertices), nbEdges(other.nbEdges){
  grapheMatrice.resize(nbVertices);
  grapheMatrice = other.grapheMatrice;
}

Graphe::~Graphe()
{
}

Graphe& Graphe::operator=(const Graphe& other)
{
  namefile = other.namefile;
  nbVertices = other.nbVertices;
  nbEdges = other.nbEdges;
  grapheMatrice.resize(nbVertices);
  grapheMatrice = other.grapheMatrice;
  return *this;
}
