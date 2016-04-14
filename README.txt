#################################################################################
#	ECP est un algorithme de recherche locale ayant	pour but de résoudre le #
#	problème de coloration équitable sur différents graphes présents dans   #
#	la littérature.								#
#	Dans un premier temps, nous implémenterons l'algorithme décrit dans	#
#	l'article "Backtracking Based Iterated Tabu Search for Equitable 	#
#	Coloring" disponible à l'adresse :					#
#	http://www.info.univ-angers.fr/pub/hao/papers/LaiHaoGloverEAAI2015.pdf  #
#################################################################################


Représentation d'un graphe
--------------------------

	Une classe a été reservée à la représentation d'un graphe G=(V,E). Cette 
	classe dispose des éléments suivants :
		- int nbVertices, nbEdges : entier représentant |V| et |E|
		- string namefile : string représentant le path du fichier source au 
		  format DIMACS du graphe étudié
		- vector< vector<bool> > G : matrice de booléen de taille |V|*|V| 
		  correspondant à la fonction f :
			f(i,j) = 1 si (i,j) ¤ E, i,j ¤ V
					 0 sinon.
		
		- un getter spécifique getMatriceValue(int x, int y) retournant la val-
		  eur de M[x][y]
		  
Coloration équitable d'un graphe
--------------------------------

	Soit le graphe G=(V,E). Une coloration équitable de ce graphe est un en-
	semble Vk de k partitions de V tel :
		- qu'aucun des sommets appartenant à une partition Vk[i] ne soit 
		  adjacent à un autre sommet de cette partition dans G. [1]
		- que la taille de chaque partition ne diffère au plus que de 1. [2]
		
	Une telle coloration étant le but de notre recherhe locale, nous nous
	attacherons à toujours respecter la contrainte [2] et à effectuer notre
	recherche via la contrainte [1].
	
	Pour représenter cette coloration, nous disposons d'une classe avec les 
	attributs suivants :
		- Graphe* G : pointeur sur le graphe à colorer
		- int nbColor : représente le nombre de couleurs utilisées.
		- vector< vector<int> > Vk : ensemble des partitions des sommets de G. 
		- vector< vector<int> > M : matrice |V|*nbColor où M[i][j] représente 
		  le nombre de sommmets adjacents au sommet i de couleur j.
		  
	Initialisation d'une coloration
	-------------------------------
	L'algorithme suivant est utilisé pour créer une coloration équitable initiale.
	
	Pour(i=0; i < nbColor; ++i){
	  selectionner un sommet Sj aléatoire dans U.
	  Vk[i] = Vk[i] + Sj.
	  U = U \ Sj.
	}
	
	i=0;
	
	Tant que ( !vide(U) ){
	  selectionner le sommet Sj ayant le moins de voisin dans la couleur i.
	  Vk[i] = Vk[i] + Sj.
	  U = U \ Sj.
	  i = (i+1) % nbColor.
	}
	
	
	Ajout de la recherche tabou
	---------------------------
	
	
	Evaluation d'une coloration
	---------------------------
	La fonction évalue le nombre de sommets en conflit. Un sommet est en conflit si
	un de voisins possède la meme couleur. Pour ce faire, nous utilisons l'algo-
	rithme suivant :
	
	total = 0.
	Pour(i=0; i < Vk.size(); ++i){
	  Pour(j=0; j < Vk[i].size(); ++j){
	    total += (int)(M[Vk[i][j]][i] != 0);
	  }
	}
	
	
	
	
		
		
