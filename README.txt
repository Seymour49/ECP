################################################################################
#																			   #
#	ECP est un algorithme de recherche locale ayant	pour but de résoudre le    #
#	problème de coloration équitable sur différents graphes présents dans 	   #
#	la littérature.															   #
################################################################################


Représentation d'un graphe
--------------------------

	Une classe a été reservée à la représentation d'un graphe G=(V,E). Cette 
	classe dispose des éléments suivants :
		- int nbVertices, nbEdges : entier représentant |V| et |E|
		- string namefile : string représentant le path du fichier source au 
		  format DIMACS du graphe étudié
		- vector< vector<bool> > G : matrice de booléen de taille |V|*|V| 
		  correspondant à la fonction f :
			f(i,j) = 1 si (i,j) € E, i,j € V
					 0 sinon.
		
		- un getter spécifique getMatriceValue(int x, int y) retournant la val-
		  eur de M[x][y]
		  
Coloration équitable d'un graphe
----------------------

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
	
		