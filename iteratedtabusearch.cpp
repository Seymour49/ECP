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

#include "iteratedtabusearch.h"
#include <random>
#include <chrono>


using namespace std;

IteratedTabuSearch::IteratedTabuSearch(Graphe* graphe, int b , int k, int d):
G(graphe), beta(b),nbColor(k),depth(d),s(G){
    
    s.initialisation(nbColor);
}

IteratedTabuSearch::IteratedTabuSearch(const IteratedTabuSearch& other):
G(other.G), beta(other.beta), nbColor(other.nbColor), depth(other.depth),
s(other.G){
}

IteratedTabuSearch::~IteratedTabuSearch(){
}


Coloration* IteratedTabuSearch::perturbate(Coloration* current){
    	
    size_t seed = chrono::system_clock::now().time_since_epoch().count();
	
    srand(seed);
    // Tirage d'un nombre aléatoire 
    int random = rand()% 100;
    cout << "random proba : " << random << endl;
    Coloration *perturbated;
    if(random < 30){
	perturbated = directedPerturbation(current);
    }
    else{
	perturbated = randomPertubation(current);
    }
    
    return perturbated;
}

Coloration* IteratedTabuSearch::randomPertubation(Coloration* current){

    Coloration* result = new Coloration(*current);
    
    // nombre de perturbations à effectuer
    int pertubationLimit = floor(  (float)result->getNbVertices() * 0.3 );
    
    // déclarations des variables aléatoires nécessaires
    int rand1, rand2, rand3, rand4;
    size_t seed;
    
    // déclarations des variables d'accès nécessaires
    int color1, color2;
    // vecteur nécéssaire à la sélection aléatoire de deux couleurs différentes
    vector<int> V;
    for(int i=0; i < result->getNbColor(); ++i){
	V.push_back(i);
    }
  
    for(int cpt = 0; cpt < pertubationLimit; ++cpt){
	    
	// On mélange V
	seed = chrono::system_clock::now().time_since_epoch().count();
	srand(seed);
	shuffle(V.begin(),V.end(),default_random_engine(seed));	
	
	rand1 = rand()% result->getNbColor();
	rand2 = rand()%(result->getNbColor() - 1);
	
	color1 = V.at(rand1);
	
	// déplacement de la couleur choisie en dernière position
	V.at(rand1) = V.at(V.size()-1);
	V.at(V.size()-1) = color1;
	
	color2 = V.at(rand2);
	cout << "nbColor : " << result->getNbColor() << endl;
	cout << "color : " << color1 << ";" << color2 << endl;
	cout << "sizes : " << result->getVkiSize(color1) << ";" << result->getVkiSize(color2) << endl;
 	rand3 = rand()%(result->getVkiSize(color1));
	rand4 = rand()%(result->getVkiSize(color2));
	
	Swap swap(result->getValueVk(color1,rand3),result->getValueVk(color2,rand4),color1,color2);
	result->validSwap(&swap);
	
		
    }
    cout << "ici aussi" << endl;
        
    return result;
}


Coloration* IteratedTabuSearch::directedPerturbation(Coloration* current){
    Coloration* result = new Coloration(*current);

    cout << "TODO implementation" << endl;
    
    return result;
}


Coloration* IteratedTabuSearch::run(){
    
    BasicTabuSearch bts(&s,depth);
    Coloration *current;
    current = bts.run();
    
    int cpt = 0;
    cout << *current;
    cout << "==================================" << endl;
    do{
	
	Coloration* prime;
	prime = perturbate(current);
	
	cout << "fin de la perturbation " << endl;
	BasicTabuSearch bts2(prime,depth);
	Coloration* second = bts2.run();
	
	if(second->evaluate() < current->evaluate() ){
	    (*current) = (*second);
	    cpt = 0;
	}
	else{
	    ++cpt;
	}
	
	delete(prime);
	delete(second);
	
    }while( (cpt < beta) && (current->evaluate() > 0) ); 
    
    return current;
}

