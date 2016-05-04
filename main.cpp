#include "iteratedtabusearch.h"
#include "binarysearch.h"
#include <chrono>
#include <time.h>

using namespace std;


int main(int argc, char **argv) {
  
  if(argc != 2) {
    cerr <<"Call the program with exact argument which is : " << endl;
    cerr << "   - $1 : path to the graph file " << endl;
    exit(EXIT_FAILURE);
  }
  else{
    
    Graphe *G = new Graphe(argv[1]);
    
    if ( G->tryLoadFile() ){
	
	int bt_depth = 4; // TODO passage en param $
	
	time_t start = time(NULL);
	
	
	BinarySearch BS(G,100);
	
	Coloration *best = BS.run();
	int kbest = best->getNbColor();
	int kcurrent = kbest;
	Coloration *current;

	do{
	    if( kcurrent == (kbest - bt_depth) ){
		kcurrent = kbest - 1;
	    }
	    else{
		--kcurrent ;
	    }
	    
	    IteratedTabuSearch its(G,30,kcurrent,10000);
	    current = its.run();
	    
	    if(current->evaluate() == 0 ){
		(*best) = (*current);
	    }

	   
	}while( difftime(time(NULL),start) < 300 );	// TODO Change to 3600 when finished	
	
	cout << *G;
	
	cout << "=========================" << endl;
	
	cout << *best;	
    
	delete(G);
    
    }else{
      exit(EXIT_FAILURE);
    }    
    cout << "Goood Jooob Guy !" << endl;    

    return 0;
  }
}
