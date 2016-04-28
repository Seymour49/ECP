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
    
    
      Coloration *C = new Coloration(G);
      C->initialisation(15);      
      cout << *C;   
      cout << "Eval init : "<< C->evaluate() << endl;
      cout << "fin affichage coloration initiale" << endl;
      BasicTabuSearch bts(*C, 20);
      
      Coloration *d = bts.run();
      
      cout << "================================"<<endl;
      cout << *d;
      
      delete(G);
      delete(C);
      delete(d);
    
    }else{
      exit(EXIT_FAILURE);
    }    
    cout << "Goood Jooob Guy !" << endl;    

    return 0;
  }
}
