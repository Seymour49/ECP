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
    // Tirage d'un nombre aléatoire 
    srand(time(NULL));
    int random = rand()% 100;
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
    
    cout << *result;
    
    return result;
}


Coloration* IteratedTabuSearch::directedPerturbation(Coloration* current){
    Coloration* result = new Coloration(*current);
    
    cout << *result;
    
    return result;
}


Coloration* IteratedTabuSearch::run(){
    
    BasicTabuSearch bts(&s,depth);
    Coloration *current;
    current = bts.run();
    
    int cpt = 0;
    
    //do{
	Coloration* prime;
	prime = perturbate(current);
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
//    }while( (cpt != beta) || (current->evaluate() != 0) ); 
    
    return current;
}

