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

#include "basictabusearch.h"
#include <cassert>

using namespace std;

BasicTabuSearch::BasicTabuSearch(Coloration* init, int prof, int maxT): 
current(*init), depth(prof), maxTenure(maxT){
    initTabuMat();
    current.initialisation(current.getNbColor());
    
    // Initialisation de ttPeriod et ttValue pour règle3 gestion tabutenure
    int values[] = {1,2,1,4,1,2,1,8,1,2,1,4,1,2,1};
    ttValue.assign(values, values+15);
    
    for(unsigned i=0; i < 15; ++i){
	ttValue[i] *= (maxTenure/8);
    }
    ttPeriod.push_back(1);
    for(unsigned i=1; i < 15; ++i){
	ttPeriod.push_back(ttPeriod[i-1] + 3*ttValue[i]);
    }
}

BasicTabuSearch::BasicTabuSearch(Coloration* init, int prof, int maxT,double rt): 
current(*init), depth(prof), maxTenure(maxT),remainingTime(rt){
    initTabuMat();
    current.initialisation(current.getNbColor());
    
    // Initialisation de ttPeriod et ttValue pour règle3 gestion tabutenure
    int values[] = {1,2,1,4,1,2,1,8,1,2,1,4,1,2,1};
    ttValue.assign(values, values+15);
    
    for(unsigned i=0; i < 15; ++i){
	ttValue[i] *= (maxTenure/8);
    }
    ttPeriod.push_back(1);
    for(unsigned i=1; i < 15; ++i){
	ttPeriod.push_back(ttPeriod[i-1] + 3*ttValue[i]);
    }
}

BasicTabuSearch::~BasicTabuSearch(){
   
    if(N.size() != 0){
	
	for(unsigned i=0; i<N.size(); ++i){
	    delete(N[i]);
	}
	N.clear();
    }
}

void BasicTabuSearch::initTabuMat(){
    if(tabuMat.size() != 0){
	tabuMat.clear();
    }
    
    // Initialisation de la matrice Tabu
    for(int i=0; i < current.getNbVertices(); ++i){
	vector<int> k;
	for(int j=0; j < current.getNbColor(); ++j){
	    k.push_back(0);
	}
	tabuMat.push_back(k);
    }
}


void BasicTabuSearch::initNeighboorOM()
{    
    for(int i=0; i< current.getNbColor(); ++i){
	
	for(int j=0; j< current.getVkiSize(i); ++j){
	    
	    // Calcul des OneMoves
	    if( current.inConflict(i,j) == true ){
		
		for(int k=0; k < current.getNbColor(); ++k){
		    
		    if( ceil( (float)current.getNbVertices()/current.getNbColor() ) != floor( (float)current.getNbVertices()/current.getNbColor() ) ){
		
			if( current.getVkiSize(i) == ceil( (float)current.getNbVertices()/current.getNbColor() ) ){
			    if( current.getVkiSize(k) == floor( ((float)current.getNbVertices()/current.getNbColor()) ) ){
				Voisin *om = new OneMove(current.getValueVk(i,j),i,k);
				N.push_back(om);
			    }
			}
		    }    
		}    
	    } 
	}
    }
}
void BasicTabuSearch::initNeighboorS()
{
    // Calcul des swaps
    for(int i=0; i < current.getNbColor(); ++i){
	
	for(int j = 0; j < current.getVkiSize(i); ++j) {
	    
	    for(int k=i+1; k < current.getNbColor(); ++k){
		
		for(int l= 0; l < current.getVkiSize(k); ++l){
		    
		    if( ( current.inConflict(k,l) == true || current.inConflict(i,j) == true ) ){
			Voisin *s;
			
			if(current.getValueVk(i,j) < current.getValueVk(k,l) ){
			    s =  new Swap(current.getValueVk(i,j),current.getValueVk(k,l), i, k);
			}else{
			    s = new Swap(current.getValueVk(k,l),current.getValueVk(i,j), k, i);
			}
			N.push_back(s);
		    }
		}		
	    }
	}
    }

}

void BasicTabuSearch::calculDelta(){

    int gain = 0;
    for(unsigned i=0; i < N.size(); ++i){
	try{
	    // Dynamic cast
	    if( dynamic_cast<OneMove*>(N[i]) == 0 ){
		gain = current.calculDeltaS(dynamic_cast<Swap*>(N[i]));
	    }else{
		gain = current.calculDeltaOM(dynamic_cast<OneMove*>(N[i]));
	    }
	    N[i]->setGain(gain);
	}catch(exception &e){
	    cerr << "Exception : " << e.what();
	    exit(EXIT_FAILURE);
	}
    }
}

bool BasicTabuSearch::isForbidden(Voisin* N, int iter){
    bool result;
    try{
	// Dynamic cast
	if( dynamic_cast<OneMove*>(N) == 0 ){
	    result = isForbiddenS(dynamic_cast<Swap*>(N),iter);
	}else{
	    result = isForbiddenOM(dynamic_cast<OneMove*>(N),iter);
	}
	
    }catch(exception &e){
	cerr << "Exception: " << e.what();
	exit(EXIT_FAILURE);
    }
 
    return result;
}

bool BasicTabuSearch::isForbiddenOM(OneMove* om, int iter){
    return ( tabuMat[om->getS()][om->getVkj()] > iter );
}

bool BasicTabuSearch::isForbiddenS(Swap* s, int iter){
    return( (tabuMat[s->getSi()][s->getKj()] > iter) || (tabuMat[s->getSj()][s->getKi()] > iter) );
}

Coloration* BasicTabuSearch::run(){
    
    Coloration *best = new Coloration(current);
    int d=0;
    int period = 0;
    int p =0;
    int iteration = 0;
    int bestEval;
    unsigned indN;
    bool chosen;
    int tabuTenure;
    
    time_t start = time(NULL);
    
    do{	
	
	if(N.size() != 0){
	    for(unsigned i=0; i < N.size(); ++i){
		delete(N[i]);
	    }
	    N.clear();
	}
	//Initialisation du voisinage
	initNeighboorS();
	initNeighboorOM();
	calculDelta();
	sort(N.begin(),N.end(), Voisin::compareGain );
	
	// Sélection d'un voisin et maj TabuMat
	chosen = false;
	indN = 0;
	bestEval = best->evaluate();
	while( (chosen == false) && (indN < N.size()-1) ){
	    int simul;
	    
	    if( isForbidden(N[indN], iteration) == false ){
		chosen = true;
	    }
	    else{
	    
		try{
		    if( dynamic_cast<OneMove*>(N[indN]) == 0 ){
			simul = current.simulEvalS(dynamic_cast<Swap*>(N[indN]));
		    }else{
			simul = current.simulEvalOM(dynamic_cast<OneMove*>(N[indN]));
		    }
		}catch(exception &e){
		    cerr << "Exception: " << e.what();
		    exit(EXIT_FAILURE);
		}
	    
		if( simul < bestEval ){
		    chosen = true;
		}else{
		    ++indN;
		}
	    }
	}
	
	// Mise à jour dynamique de la tabuTenure
	if( iteration < (100000/3)){
	    // Règle 1 : tt = C0 + rand() % C1
	    tabuTenure = 5 + rand() % 5;	    
	}
	else if( iteration < (100000*2/3) ){
	    // Règle 2 : tt = alpha * |C(s)| + rand()%beta
	    int cs = current.evaluate();
	    tabuTenure = 0.9*cs + rand()% 5;
	}
	else{
	    // Règle 3 : voir papier
	    if(p < ttPeriod[period] ){
		++p;
	    }else{
		
		if(period == 14){
		    period = 0;
		}else{
		    ++period;
		}
		p = 0;
	    }
	    tabuTenure = ttValue[period];
	    
	}
	// Le voisin choisi est N[indN], reste à mettre à jour tabuMat
	try{
	    if( dynamic_cast<Swap*>(N[indN]) == 0 ){
		// OneMove
		OneMove * om = dynamic_cast<OneMove*>(N[indN]);
		tabuMat[om->getS()][om->getVki()] = iteration + tabuTenure;
		current.validOneMove(om);
		
	    }else{
		Swap* s = dynamic_cast<Swap*>(N[indN]);
		tabuMat[s->getSi()][s->getKi()] = iteration + tabuTenure;
		tabuMat[s->getSj()][s->getKj()] = iteration + tabuTenure;
		current.validSwap(s);
	    }
	}catch(exception &e){
	     cerr << "Exception: " << e.what();
	     exit(EXIT_FAILURE);
	}
	
	if( current.evaluate() < bestEval ){
	    
	    (*best) = current; 
	    bestEval = best->evaluate();
	    d = 0;
	}else{
	    ++d;
	}
	
	++iteration;
	
    }while( (d < depth && current.evaluate() > 0) && (difftime(time(NULL),start) < remainingTime) );
    
    
    // On réinitialise tabuMat
    initTabuMat();
    return best;
	
}

Coloration* BasicTabuSearch::runwithoutTL()
{
    Coloration *best = new Coloration(current);
    int d=0;
    int period = 0;
    int p =0;
    int iteration = 0;
    int bestEval;
    unsigned indN;
    bool chosen;
    int tabuTenure;
    
    do{
	
	if(N.size() != 0){
	    for(unsigned i=0; i < N.size(); ++i){
		delete(N[i]);
	    }
	    N.clear();
	}
	
	//Initialisation du voisinage
	initNeighboorS();
	initNeighboorOM();
	calculDelta();
	sort(N.begin(),N.end(), Voisin::compareGain );
	
	// Sélection d'un voisin et maj TabuMat
	chosen = false;
	indN = 0;
	bestEval = best->evaluate();
	if( N.size() != 0 ){
	    while( (chosen == false) && (indN < N.size()) ){
		int simul;
		
		if( isForbidden(N[indN], iteration) == false ){
		    chosen = true;
		}
		else{
		
		    try{
			if( dynamic_cast<OneMove*>(N[indN]) == 0 ){
			    simul = current.simulEvalS(dynamic_cast<Swap*>(N[indN]));
			}else{
			    simul = current.simulEvalOM(dynamic_cast<OneMove*>(N[indN]));
			}
		    }catch(exception &e){
			cerr << "Exception: " << e.what();
			exit(EXIT_FAILURE);
		    }
		
		    if( simul < bestEval ){
			chosen = true;
		    }else{
			++indN;
		    }
		}
	    }
	    
	    // Mise à jour dynamique de la tabuTenure
	    if( iteration < (100000/3)){
		// Règle 1 : tt = C0 + rand() % C1
		tabuTenure = 5 + rand() % 5;	    
	    }
	    else if( iteration < (100000*2/3) ){
		// Règle 2 : tt = alpha * |C(s)| + rand()%beta
		int cs = current.evaluate();
		tabuTenure = 0.9*cs + rand()% 5;
	    }
	    else{
		// Règle 3 : voir papier
		if(p < ttPeriod[period] ){
		    ++p;
		}else{
		    
		    if(period == 14){
			period = 0;
		    }else{
			++period;
		    }
		    p = 0;
		}
		tabuTenure = ttValue[period];
		
	    }
	    // Le voisin choisi est N[indN], reste à mettre à jour tabuMat
	    try{
		if( dynamic_cast<Swap*>(N[indN]) == 0 ){
		    // OneMove
		    OneMove * om = dynamic_cast<OneMove*>(N[indN]);
		    tabuMat[om->getS()][om->getVki()] = iteration + tabuTenure;
		    current.validOneMove(om);
		    
		}else{
		    Swap* s = dynamic_cast<Swap*>(N[indN]);
		    tabuMat[s->getSi()][s->getKi()] = iteration + tabuTenure;
		    tabuMat[s->getSj()][s->getKj()] = iteration + tabuTenure;
		    current.validSwap(s);
		}
	    }catch(exception &e){
		cerr << "Exception: " << e.what();
		exit(EXIT_FAILURE);
	    }
	}
	
	if( current.evaluate() < bestEval ){
	    
	    (*best) = current; 
	    bestEval = best->evaluate();
	    d = 0;
	}else{
	    ++d;
	}
	
	++iteration;
	
    }while( (d < depth && current.evaluate() > 0) );
    
    // On réinitialise tabuMat
    initTabuMat();
    return best;
	
}
