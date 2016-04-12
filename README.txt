################################################################################
#																			   #
#	ECP est un algorithme de recherche locale ayant	pour but de r�soudre le    #
#	probl�me de coloration �quitable sur diff�rents graphes pr�sents dans 	   #
#	la litt�rature.															   #
################################################################################


Repr�sentation d'un graphe
--------------------------

	Une classe a �t� reserv�e � la repr�sentation d'un graphe G=(V,E). Cette 
	classe dispose des �l�ments suivants :
		- int nbVertices, nbEdges : entier repr�sentant |V| et |E|
		- string namefile : string repr�sentant le path du fichier source au 
		  format DIMACS du graphe �tudi�
		- vector< vector<bool> > G : matrice de bool�en de taille |V|*|V| 
		  correspondant � la fonction f :
			f(i,j) = 1 si (i,j) � E, i,j � V
					 0 sinon.
		
		- un getter sp�cifique getMatriceValue(int x, int y) retournant la val-
		  eur de M[x][y]
		  
Coloration �quitable d'un graphe
----------------------

	Soit le graphe G=(V,E). Une coloration �quitable de ce graphe est un en-
	semble Vk de k partitions de V tel :
		- qu'aucun des sommets appartenant � une partition Vk[i] ne soit 
		  adjacent � un autre sommet de cette partition dans G. [1]
		- que la taille de chaque partition ne diff�re au plus que de 1. [2]
		
	Une telle coloration �tant le but de notre recherhe locale, nous nous
	attacherons � toujours respecter la contrainte [2] et � effectuer notre
	recherche via la contrainte [1].
	
	Pour repr�senter cette coloration, nous disposons d'une classe avec les 
	attributs suivants :
	
		