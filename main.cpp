#include "iteratedtabusearch.h"
#include "binarysearch.h"
#include <chrono>
#include <sstream>
#include <fstream>
#include <time.h>

using namespace std;
/**
 * Ce programme implémente l'algorithme BITS décrit dans l'article
 * cosigné par MMr. Xiangjing Lai, Jin-Kao Hao et Fred Glover et 
 * disponible à l'adresse suivante :
 * 
 * http://www.info.univ-angers.fr/pub/hao/papers/LaiHaoGloverEAAI2015.pdf
 * 
 */ 


int main(int argc, char **argv) {
  
  if(argc != 4) {
    cerr <<"Call the program with exact argument which are : " << endl;
    cerr << "   - $1 : path to the graph file " << endl;
    cerr << "   - $2 : time accorded to the execution (seconds)" << endl;
    cerr << "   - $3 : execution count" << endl;
    exit(EXIT_FAILURE);
  }
  else{
    
    Graphe *G = new Graphe(argv[1]);
    double totalTime = atof(argv[2]);
    int cpt = atoi(argv[3]);
    
    if ( G->tryLoadFile() ){

	int bt_depth = 4;
	    
	for(int i = 0; i < cpt; ++i){
	    time_t start = time(NULL);
		    
	    cout << "Début de la recherche binaire" << endl;
	    BinarySearch BS(G,100);
	    Coloration *best = BS.run();
	    
	    int kbest = best->getNbColor();
	    int kcurrent = kbest;
	    cout << "Fin de la recherche binaire, kbest = " << kbest <<". remainingTime : " << totalTime-difftime(time(NULL),start) << endl;
	    
	    double remainingTime;
	    
	    do{
		Coloration *current;
		remainingTime = totalTime - difftime(time(NULL),start);
		
		if( kcurrent == (kbest - bt_depth) || kcurrent == 2 ){
		    kcurrent = kbest - 1;
		}else{
		    --kcurrent;
		}
		
		IteratedTabuSearch its(G,30,kcurrent,100000,remainingTime);
		current = its.run();
		cout << "Fin ITS avec " << kcurrent << "colors";
		
		if(current->evaluate() == 0 ){
		    (*best) = (*current);
		    kbest = current->getNbColor();
		    cout << " avec succes" ;
		}
		cout << ". RemainingTime : " << totalTime-difftime(time(NULL),start) << endl;
		
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
	}
	delete(G);

    }else{
      exit(EXIT_FAILURE);
    }
    cout << "Goood Jooob Guy !" << endl;    

    return 0;
  }
}
