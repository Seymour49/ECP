#include "voisin.h"
#include "onemove.h"
#include "swap.h"
#include "graphe.h"
#include "coloration.h"

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
      C->initialisation(/*floor((float)G->getNbVertices()/2)*/3);
      C->initNeighboor();
      //C->calculDelta();
      
      cout << *C;
      
      cout << "Nombre de sommets en conflits : " << C->evaluate() << endl;
      

      delete(C);
    
    }else{
      exit(EXIT_FAILURE);
    }    
    cout << "Goood Jooob Guy !" << endl;    

    return 0;
  }
}
