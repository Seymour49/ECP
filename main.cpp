#include "iteratedtabusearch.h"
#include "binarysearch.h"
#include <chrono>
#include <sstream>
#include <fstream>
#include <time.h>

using namespace std;
/**
 * Ce
 * 
 * 
 */ 


int main(int argc, char **argv) {
  
  if(argc != 3) {
    cerr <<"Call the program with exact argument which is : " << endl;
    cerr << "   - $1 : path to the graph file " << endl;
    cerr << "   - $2 : time accorded to the execution (seconds)" << endl;
    exit(EXIT_FAILURE);
  }
  else{
    
    Graphe *G = new Graphe(argv[1]);
    double totalTime = atof(argv[2]);
    
    if ( G->tryLoadFile() ){

	
	int bt_depth = 4; // TODO passage en param $
	
	time_t start = time(NULL);
	
	cout << "Début de la recherche binaire" << endl;
	BinarySearch BS(G,100);
	Coloration *best = BS.run();
	
	int kbest = best->getNbColor();
	int kcurrent = kbest;
	
	cout << "Fin de la recherche binaire, kbest = " << kbest << endl;
	
	double remainingTime;
	do{
	    Coloration *current;
	    
	    remainingTime = totalTime - difftime(time(NULL),start);
	    if( kcurrent == (kbest - bt_depth) ){
		kcurrent = kbest - 1;
	    }
	    else{
		--kcurrent ;
	    }
	    
	    IteratedTabuSearch its(G,30,kcurrent,25000,remainingTime);
	    current = its.run();
	    
	    if(current->evaluate() == 0 ){
		(*best) = (*current);
		kbest = current->getNbColor();
	    }
	    cout << "Fin ITS avec " << kcurrent << "color" << endl;
	   delete(current);
	}while( difftime(time(NULL),start) < totalTime);
	

	// Création du fichier de sortie avec le timestamp
	ostringstream oss;
	oss << start;
	string instanceName = argv[1];
	instanceName = instanceName.substr(13,instanceName.length()-1);
	string resFileName = "../results/"+oss.str()+"_"+instanceName;
	ofstream file(resFileName);
	
	cout << "Fin de la recherche locale itérée, résultat consultable dans " + resFileName << endl;

	if(!file){
	    cerr << "Erreur de création du fichier" << endl;
	}
	else{
	    file << " Execution de l'algoritme BITS sur le fichier " + instanceName << endl;
	    file << "=====================================================================" << endl;
	    file << *best;
	    cout << "Ecriture réussie" << endl;
	}

	delete(best);	
	delete(G);
    
    
    }else{
      exit(EXIT_FAILURE);
    }
    cout << "Goood Jooob Guy !" << endl;    

    return 0;
  }
}
