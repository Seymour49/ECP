#################################################################################
#	ECP est un algorithme de recherche locale ayant	pour but de r�soudre le #
#	probl�me de coloration �quitable sur diff�rents graphes pr�sents dans   #
#	la litt�rature.								#
#	Dans un premier temps, nous impl�menterons l'algorithme d�crit dans	#
#	l'article "Backtracking Based Iterated Tabu Search for Equitable 	#
#	Coloring" disponible � l'adresse :					#
#	http://www.info.univ-angers.fr/pub/hao/papers/LaiHaoGloverEAAI2015.pdf  #
#################################################################################


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
--------------------------------

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
		- Graphe* G : pointeur sur le graphe � colorer
		- int nbColor : repr�sente le nombre de couleurs utilis�es.
		- vector< vector<int> > Vk : ensemble des partitions des sommets de G. 
		- vector< vector<int> > M : matrice |V|*nbColor o� M[i][j] repr�sente 
		  le nombre de sommmets adjacents au sommet i de couleur j.
		  
	Initialisation d'une coloration
	-------------------------------
	L'algorithme suivant est utilis� pour cr�er une coloration �quitable initiale.
	
	Pour(i=0; i < nbColor; ++i){
	  selectionner un sommet Sj al�atoire dans U.
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
	La fonction �value le nombre de sommets en conflit. Un sommet est en conflit si
	un de voisins poss�de la m�me couleur. Pour ce faire, nous utilisons l'algo-
	rithme suivant :
	
	total = 0.
	Pour(i=0; i < Vk.size(); ++i){
	  Pour(j=0; j < Vk[i].size(); ++j){
	    total += (int)(M[Vk[i][j]][i] != 0);
	  }
	}
	
	
	D�finition du voisinage
	-----------------------
	Le voisinage d'une solution s est l'union de deux voisinages d�crits comme suit :
	
	N(s) = N1(s) U N2(s) o� :
	  - N1(s) = { <Sj, Vi,Vj> | Sj � Vi n C(s), i != j, |Vk[i]| > floor(|V|/nbColor),
	    |Vk[j]| < top(|V|/nbColor) } o� C(s) repr�sente l'ensemble des sommets en 
	    conflits de la solution courante s.
	  
	  - N2(s) = { (Si,Sj) | Si � Vk[i], Sj � Vk[j], i != j, {Si,Sj} n C(s) != {}  }
	  
	Plusieurs choix s'offrent � nous pour la repr�sentation de ces deux voisinages. 
	En effet, nous pouvons � la fois d�finir une classe abstraite N puis deux sous-classes
	N1 et N2 impl�mentant chacun un voisinage puis travailler sur un ensemble d'objets de 
	type N. 
	Nous pourrions �galement travailleur sur deux vecteurs diff�rents repr�sentant chacun
	un voisinage.
	De nombreuses autres impl�mentations sont possibles et nous reviendrons dessus si le 
	temps nous le permet.
	Nous choisirons dans un premier temps de travailler sur deux vecteurs diff�rents afin
	d'anticiper la cr�ation de la m�thode de perturbation.
	
	De plus, la recherche tabou pr�sent�e utilisant une strat�gie de s�lection du meilleur
	am�liorant,nous ajouterons � chacun des voisins le score de gain qui lui est associ�.
	Nous pr�senterons le calcul de ce gain dans la prochaine section.
	Enfin, nous impl�mentons deux structures de donn�es que nous stockerons dans deux
	vecteurs :
	
	    - typedef struct oneMove�{
		int gain;
		int Sj;
		int Vi;
		int Vj;
	      } oneMove;
	      
	    - typedef struct swap {
		int gain;
		int Si;
		int Sj;
	      } swap;
	      
	    vector< oneMove *> N1;
	    vector< swap *> N2;
	  
	Mise � jour de M apr�s un mouvement
	-----------------------------------
	Apr�s avoir effectu� une op�ration de voisinage, la matrice M a besoin d'�tre mise �
	jour. Deux fonctions sont donc d�finies :
	
	UpdateMafterOneMove(int Sj, int Ci, int Cj){
	  
	  
	}
	
	UpdateMafterSwap(int Si, int Sj){
	
	
	}
	
	
		
		
