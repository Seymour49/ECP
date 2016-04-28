#include "basictabusearch.h"

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
    
      cout << *G;
      cout << "***********************************" << endl;
      Coloration *C = new Coloration(G);
      C->initialisation(15);      

      cout << "Coloration initiale : " << endl;
      cout << *C;
      
      BasicTabuSearch bts(C, 10000);
      Coloration *d = nullptr;
      d = bts.run();
      
      cout << "================================"<<endl;
      cout << "Coloration finale : " << endl;
      cout << (*d);
      
      
      
      delete(d);
      delete(G);
    
    }else{
      exit(EXIT_FAILURE);
    }    
    cout << "Goood Jooob Guy !" << endl;    

    return 0;
  }
}
