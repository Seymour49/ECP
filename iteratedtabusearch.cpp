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


using namespace std;

IteratedTabuSearch::IteratedTabuSearch(Graphe* graphe, int b , int k, int d,double rt):
G(graphe), beta(b),nbColor(k),depth(d),s(G),remainingTime(rt){
    
    s.initialisation(nbColor);
}

IteratedTabuSearch::IteratedTabuSearch(const IteratedTabuSearch& other):
G(other.G), beta(other.beta), nbColor(other.nbColor), depth(other.depth),
s(other.G),remainingTime(other.remainingTime){
}

IteratedTabuSearch::~IteratedTabuSearch(){
}


void IteratedTabuSearch::perturbate(Coloration* prime){
    
    size_t seed = chrono::system_clock::now().time_since_epoch().count();
	
    srand(seed);
    // Tirage d'un nombre aléatoire 
    int random = rand()% 100;
   
    if(random < 30){
	cout << "Perturbation dirigée." << endl;
	directedPerturbation(prime);
    }
    else{
	cout << "Perturbation aléatoire." << endl;
	randomPertubation(prime);
    }
    
}

void IteratedTabuSearch::randomPertubation(Coloration* prime){
    
    // nombre de perturbations à effectuer
    int pertubationLimit = floor(  (float)prime->getNbVertices() * 0.3 );
    
    // déclarations des variables aléatoires nécessaires
    int rand1, rand2, rand3, rand4;
    size_t seed;
    
    // déclarations des variables d'accès nécessaires
    int color1, color2;
    // vecteur nécéssaire à la sélection aléatoire de deux couleurs différentes
    vector<int> V;
    for(int i=0; i < prime->getNbColor(); ++i){
	V.push_back(i);
    }
  
    for(int cpt = 0; cpt < pertubationLimit; ++cpt){
	    
	// On mélange V
	seed = chrono::system_clock::now().time_since_epoch().count();
	srand(seed);
	shuffle(V.begin(),V.end(),default_random_engine(seed));	
	
	rand1 = rand()% prime->getNbColor();
	rand2 = rand()%(prime->getNbColor() - 1);
	
	color1 = V.at(rand1);
	
	// déplacement de la couleur choisie en dernière position
	V.at(rand1) = V.at(V.size()-1);
	V.at(V.size()-1) = color1;
	
	color2 = V.at(rand2);
	
 	rand3 = rand()%(prime->getVkiSize(color1));
	rand4 = rand()%(prime->getVkiSize(color2));
	
	Swap swap(prime->getValueVk(color1,rand3),prime->getValueVk(color2,rand4),color1,color2);
	prime->validSwap(&swap);
	
    }
        
}


void IteratedTabuSearch::directedPerturbation(Coloration* prime){
    
    // Variables
    int pertubationLimit = 500;
    bool chosen;
    int bestEval;
    unsigned ind;
    int tabuTenure;
    vector<Voisin *> Neighboor;
    
    vector<vector<int> >tabuMat;    
    for(int i=0; i < prime->getNbVertices(); ++i){
	vector<int> k;
	for(int j=0;j<prime->getNbColor(); ++j){
	    k.push_back(0);
	}
	tabuMat.push_back(k);
    }   
    
    // Boucle principale
    int iter = 0;
    while(iter < pertubationLimit && prime->evaluate() != 0){
//     for(int iter=0; iter < pertubationLimit; ++iter){
	
	if( Neighboor.size() > 0){
	    for(unsigned l=0; l < Neighboor.size(); ++l){
		delete(Neighboor[l]);
	    }
	    Neighboor.clear();
	}
	
	// Initialisation du voisinage
	for(int i=0; i < prime->getNbColor(); ++i){
	    for(int j=0; j < prime->getVkiSize(i); ++j){
		
		// calcul des swaps
		for(int k = i+1; k < prime->getNbColor(); ++k){
		    for(int l=0; l < prime->getVkiSize(k); ++l){
			
			if( (prime->inConflict(k,l) || prime->inConflict(i,j)) ){
			    Voisin *s;
			    
			    if(prime->getValueVk(i,j) < prime->getValueVk(k,l) ){
				s = new Swap(prime->getValueVk(i,j), prime->getValueVk(k,l), i, k);
			    }
			    else{
				s = new Swap(prime->getValueVk(k,l), prime->getValueVk(i,j), k, i);
			    }
			    Neighboor.push_back(s);
			}
			
		    }
		}
		
		// Calcul des OneMoves
		if(prime->inConflict(i,j) ){
		    int top = ceil( (float)prime->getNbVertices()/prime->getNbColor() );
		    int bot = floor( (float)prime->getNbVertices()/prime->getNbColor() );
		    
		    for( int m=0; m < prime->getNbColor(); ++m){
			if( top != bot ){
			    if( prime->getVkiSize(i) == top && prime->getVkiSize(m) == bot){
				Voisin* om = new OneMove(prime->getValueVk(i,j),i,m);
				Neighboor.push_back(om);
			    }
			}
		    }
		}
	    }	    
	}
	calculDelta(prime, Neighboor);
	sort(Neighboor.begin(),Neighboor.end(), Voisin::compareGain );

	chosen = false;
	ind = 0;
	bestEval = prime->evaluate();
	
	while( (chosen == false) && (ind < Neighboor.size()-1) ){
	    int simul;
	    
	    if( isForbidden(Neighboor[ind], iter, tabuMat) == false) {
		chosen = true;
	    }else{
		
		try{ 
		    if( dynamic_cast<OneMove*>(Neighboor[ind]) == 0){
			simul = prime->simulEvalS(dynamic_cast<Swap*>(Neighboor[ind]) );
		    }else{
			simul = prime->simulEvalOM(dynamic_cast<OneMove*>(Neighboor[ind]));
		    }		
		}catch(exception &e){
		    cerr << "Exception : " << e.what();
		    exit(EXIT_FAILURE);
		}
		if( simul < bestEval ){
		    chosen = true;
		}else{
		    ind++;
		}
	    }
	}
	
	// Maj tabuTenure et solution courante
	tabuTenure = rand()% 1000 + 2000;
	
	try{
	    // Dynamic_cast
	    if( dynamic_cast<OneMove*>(Neighboor[ind]) == 0){
		Swap* s = dynamic_cast<Swap*>(Neighboor[ind]);
		tabuMat[s->getSi()][s->getKi()] = iter + tabuTenure;
		tabuMat[s->getSj()][s->getKj()] = iter + tabuTenure;
		
		// Mise à jour de M
		for(int i=0; i< prime->getNbVertices();++i){
		    if( prime->areAdjacent(s->getSi(),i) ){
			prime->decreaseM(i,s->getKi());
			prime->increaseM(i,s->getKj());
		    }
		    
		    if( prime->areAdjacent(s->getSj(),i) ){
			prime->decreaseM(i,s->getKj());
			prime->increaseM(i,s->getKi());
		    }
		}
		// Déplacement des sommets
		prime->moveM(s->getSi(), s->getKi(), s->getKj());
		prime->moveM(s->getSj(), s->getKj(), s->getKi());
		
	    }else{
		OneMove* om = dynamic_cast<OneMove*>(Neighboor[ind]);
		tabuMat[om->getS()][om->getVki()] = iter + tabuTenure;
		
		// Mise à jour de M
		for(int i=0; i< prime->getNbVertices();++i){
		    if( prime->areAdjacent(om->getS(),i) ){
			prime->decreaseM(i,om->getVki());
			prime->increaseM(i,om->getVkj());
		    }
		}
		// Déplacement du sommet
		prime->moveM(om->getS(), om->getVki(), om->getVkj());
	    }
	    
	}catch(exception &e){
	    cerr << "Exception : " << e.what();
	    exit(EXIT_FAILURE);
	}
	++iter;	
    }

    if( Neighboor.size() > 0){
	for(unsigned l=0; l < Neighboor.size(); ++l){
	    delete(Neighboor[l]);
	}
	Neighboor.clear();
	
    }

}

void IteratedTabuSearch::initNeighboor(Coloration* current, std::vector< Voisin* > Neighboor){
    
    for(int i=0; i < current->getNbColor(); ++i){
	for(int j=0; j < current->getVkiSize(i); ++j){
	    
	    // Calcul des swaps
	    for(int k = i+1; k < current->getNbColor(); ++k){
		for(int l=0; l < current->getVkiSize(k); ++l){
		    
		    if( (current->inConflict(k,l) || current->inConflict(i,j)) ){
			Voisin *s;
			
			if(current->getValueVk(i,j) < current->getValueVk(k,l) ){
			    s = new Swap(current->getValueVk(i,j), current->getValueVk(k,l), i, k);
			}else{
			    s = new Swap(current->getValueVk(k,l), current->getValueVk(i,j), k, i);
			}
			Neighboor.push_back(s);
		    }
		}
	    }
	    
	    // Calcul des OneMoves
	    if( current->inConflict(i,j) ){
		int top = ceil( (float)current->getNbVertices()/current->getNbColor() );
		int bot = floor( (float)current->getNbVertices()/current->getNbColor() );
		
		for(int k=0; k < current->getNbColor(); ++k){
		    if( top != bot ){
			if(current->getVkiSize(i) == top ){
			    
			    if(current->getVkiSize(k) == bot ){
				Voisin* om = new OneMove(current->getValueVk(i,j),i,k);
				Neighboor.push_back(om);
			    }
			}
		    }
		}
	    }
	}
    }

}

void IteratedTabuSearch::calculDelta(Coloration* current, std::vector< Voisin* > Neighboor){
    
    int gain = 0;
    for(unsigned i=0; i < Neighboor.size(); ++i){
	try{
	    // Dynamic_cast
	    if( dynamic_cast<OneMove*>(Neighboor[i]) == 0 ){
		gain = current->calculDeltaS(dynamic_cast<Swap*>(Neighboor[i]));
	    }else{
		gain = current->calculDeltaOM(dynamic_cast<OneMove*>(Neighboor[i]));
	    }
	    Neighboor[i]->setGain(gain);
	}catch(exception &e){
	    cerr << "Exception : " << e.what();
	    exit(EXIT_FAILURE);
	}
    }
}

bool IteratedTabuSearch::isForbidden(Voisin* Neighboor, int iter, vector< vector< int > >& tabuMat){
    bool result;
    try{
	// Dynamic_cast
	if( dynamic_cast<OneMove*>(Neighboor) == 0){
	    result = isForbiddenS(dynamic_cast<Swap*>(Neighboor),iter,tabuMat);
	}else{
	    result = isForbiddenOM(dynamic_cast<OneMove*>(Neighboor),iter,tabuMat);
	}
    }catch(exception &e){
	cerr << "Exception : " << e.what();
	exit(EXIT_FAILURE);
    }
    
    return result;
}

bool IteratedTabuSearch::isForbiddenOM(OneMove* om, int iter, vector< vector< int > >& tabuMat){
    return ( tabuMat[om->getS()][om->getVkj()] > iter );
}

bool IteratedTabuSearch::isForbiddenS(Swap* s, int iter, vector< vector< int > >& tabuMat){
    return (	tabuMat[s->getSi()][s->getKj()] > iter ||
		tabuMat[s->getSj()][s->getKi()] > iter );
}


Coloration* IteratedTabuSearch::run(){
    
    time_t start = time(NULL);
    BasicTabuSearch bts(&s,depth,80,remainingTime);
    Coloration *current;
    current = bts.run();
    
    int cpt = 0;
    double remainingT;
    
    do{
	remainingT = remainingTime - difftime(time(NULL),start);
	Coloration* prime = new Coloration(*current);
	perturbate(prime);
	
	BasicTabuSearch bts2(prime,depth,80,remainingT);
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
	
    }while( (cpt < beta) && (current->evaluate() > 0) && (difftime(time(NULL),start) < remainingTime) ); 
    
    return current;
}

