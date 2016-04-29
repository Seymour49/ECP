#include "basictabusearch.h"
#include <chrono>


using namespace std;


int main(int argc, char **argv) {
  
  if(argc != 3) {
    cerr <<"Call the program with exact argument which is : " << endl;
    cerr << "   - $1 : path to the graph file " << endl;
    cerr << "	- $2 : initial k-value	" << endl;
    exit(EXIT_FAILURE);
  }
  else{
    
    Graphe *G = new Graphe(argv[1]);
    
    if ( G->tryLoadFile() ){
   
//       cout << *G;
//       cout << "***********************************" << endl;
      Coloration *C = new Coloration(G);
      
      C->initialisation(atoi(argv[2]));      

//       cout << "Coloration initiale : " << endl;
//       cout << *C;
      
      // Déclaration du timer
      chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();	// Timer start
      
      BasicTabuSearch bts(C, 10000);
      Coloration *d = nullptr;
      d = bts.run();
      
      // Fin de la partie essentielle de notre algo, on arrête le timer ici
      chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();	// Timer fin
	    
      auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
      
      
      cout << "exec time : " << duration << " milliseconds. nbConflicts : " << d->evaluate() << endl;
//       cout << "================================"<<endl;
//       cout << "Coloration finale : " << endl;
//       cout << (*d);
      
      
      
      delete(d);
      delete(G);
    
    }else{
      exit(EXIT_FAILURE);
    }    
    cout << "Goood Jooob Guy !" << endl;    

    return 0;
  }
}
