
#include "graphe.h"
#include "coloration.h"

using namespace std;



int main(int argc, char **argv) {
  ///*
  if(argc != 2) {
    cerr <<"Call the program with exact argument which is : " << endl;
    cerr << "   - $1 : path to the graph file " << endl;
    exit(EXIT_FAILURE);
  }
  else{
    //*/
    Graphe *G = new Graphe(argv[1]);
  
    if ( G->tryLoadFile() ){
      
      Coloration C(G);
      C.initialisation(floor(G->getNbVertices()/2));
      
      cout << C;
      
      cout << "Nombre de sommets en conflits : " << C.evaluate() << endl;

      
    }else{
      exit(EXIT_FAILURE);
    }    
    cout << "Goood Jooob Guy !" << endl;
  }  
    return 0;
}
