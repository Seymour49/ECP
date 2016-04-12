
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
      //cout << *G;
      Coloration C(G,5);
      
      cout << C;
     
    
      

      
    }else{
      exit(EXIT_FAILURE);
    }    
    cout << "Goood Jooob Guy !" << endl;
  }  
    return 0;
}
