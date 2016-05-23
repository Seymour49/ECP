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

#include "binarysearch.h"

using namespace std;

BinarySearch::BinarySearch(Graphe* graphe, int d):
G(graphe), depth(d),s(G){
    
}

BinarySearch::BinarySearch(const BinarySearch& other):
G(other.G), depth(other.depth),s(other.G){
}

BinarySearch::~BinarySearch(){
}

Coloration* BinarySearch::run(){
    
    int upper = G->getNbVertices();
    int lower = 0;
    int k;
    Coloration *best = new Coloration(s);
    
    while( upper > (lower + 1) ){
	k = floor( (float)(upper+lower)/2.0);
	s.initialisation(k);
	
	BasicTabuSearch bts(&s,depth,80);
	Coloration *d;
	d = bts.run();
	
	if(d->evaluate() == 0 ){
	    (*best) = (*d);
	    upper = k;
	}else{
	    lower = k;
	}
	delete(d);
	
    }

    return best;
}
