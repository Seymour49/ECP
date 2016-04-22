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
	un de voisins possède la même couleur. Pour ce faire, nous utilisons l'algo-
	rithme suivant :
	
	total = 0.
	Pour(i=0; i < Vk.size(); ++i){
	  Pour(j=0; j < Vk[i].size(); ++j){
	    total += (int)(M[Vk[i][j]][i] != 0);
	  }
	}
	
	
	Définition du voisinage
	-----------------------
	Le voisinage d'une solution s est l'union de deux voisinages décrits comme suit :
	
	N(s) = N1(s) U N2(s) où :
	  - N1(s) = { <Sj, Vi,Vj> | Sj ¤ Vi n C(s), i != j, |Vk[i]| > floor(|V|/nbColor),
	    |Vk[j]| < top(|V|/nbColor) } où C(s) représente l'ensemble des sommets en 
	    conflits de la solution courante s.
	  
	  - N2(s) = { (Si,Sj) | Si ¤ Vk[i], Sj ¤ Vk[j], i != j, {Si,Sj} n C(s) != {}  }
	  
	Plusieurs choix s'offrent à nous pour la représentation de ces deux voisinages. 
	En effet, nous pouvons à la fois définir une classe abstraite N puis deux sous-classes
	N1 et N2 implémentant chacun un voisinage puis travailler sur un ensemble d'objets de 
	type N. 
	Nous pourrions également travailleur sur deux vecteurs différents représentant chacun
	un voisinage.
	De nombreuses autres implémentations sont possibles et nous reviendrons dessus si le 
	temps nous le permet.
	Nous choisirons dans un premier temps de travailler sur un seul vecteur d'éléments voisins.
	
	De plus, la recherche tabou présentée utilisant une stratégie de sélection du meilleur
	améliorant,nous ajouterons à chacun des voisins le score de gain qui lui est associé.
	Nous présenterons le calcul de ce gain dans la prochaine section.
	Ainsi, nous disposons d'une classe "mère" Voisin, disposant d'un gain et de deux classes
	"filles" implémentant les voisinages OneMove et Swap. Nous n'utiliserons donc qu'un
	seul vecteur.
	
	    - class Voisin {
	    	[...]
	    	private:
	    	   int gain;
	    }
	    
	    - class OneMove : public Voisin{
	        [...]
		private:
	          int Sj;
		  int Vi;
		  int Vj;
	      };
	      
	    - class Swap :public Voisin {
	    	[...]
		private:
		  int Si;
		  int Sj;
		  int Ki;
		  int Kj;
	      };
	      
	    vector< oneMove *> N1;
	    vector< swap *> N2;
	  
	Calcul du gain
	--------------
	Afin de sélectionner le meilleur améliorant, nous avons mis en place une procédure 
	d'évaluation d'un voisin v dépendant du type de mouvement choisi. Cette procédure
	est basée sur la matrice  M définie précedemment.
	
	- OneMove : delta(<s,i,j>) : M[s][Vk[j]] - M[s][Vk[i]]
	
	- Swap : delta(<u,v,ku,kv>) : M[v][ku] - M[v][kv] + (M[u][kv] - M[u][ku] - 2*G[u][v])
		
	Mise à jour de M après un mouvement
	-----------------------------------
	Après avoir effectué une opération de voisinage, la matrice M a besoin d'être mise à
	jour. Deux fonctions sont donc définies :
	
	UpdateMafterOneMove(int Sj, int Ci, int Cj){
	  
	  
	}
	
	UpdateMafterSwap(int Si, int Sj, int Ki, int Kj){
	
	
	}
	
	Gestion de la liste tabou
	-------------------------
	Une fois un mouvement de voisinage effectué et la matrice M mise à jour, il convient
	d'interdire au(x) sommet(s) ayant changé de classe de couleur d'y revenir avant un 
	certain nombre d'itérations, ce via la mise en place d'une matrice tabou.
	De fait, si un voisin OneMove <S,Vi,Vj> est sélectionné, le mouvement suivant est 
	interdits :
		- (S,Vi)
	Si le voisin appliqué est un Swap <Si,Sj,Ki,Kj>, les mouvements suivants sont interdits :
		- (Si,Ki)
		- (Sj,Kj)
		
	Ainsi, cette matrice tabou de taille |V|*k est telle que MT[u][k] représente l'itération
	jusqu'à laquelle le mouvement consistant à déplacer le sommet u vers la couleur k a été
	ou sera interdit. Nous devons donc définir une fonction pour vérifier qu'un mouvement 
	n'est pas interdit et une fonction pour mettre à jour l'itération jusqu'à laquelle
	sera interdit un mouvement après son application. Soit ß le nombre d'itérations pendant
	lequel un mouvement est interdit après application.
	Bien entendu, chaque fonction possèdera deux définitions selo le type de voisin choisi.
	
	isForbiddenOneMove(int S,int Kj, int iter){
		return MT[S][Kj] > iter;
	}
	
	isForbiddenSwap(int Si, int Sj, int Ki, int Kj, int iter){
		return ( MT[Si][Kj] > iter || MT[Sj][Ki] > iter );
	}
		
	updateMTafterOneMove(int S, int Ki, int iter){
		MT[s][Ki] = iter + ß;
	}
	
	udateMTafterSwap(int Si, int Sj, int Ki, int Kj, int iter){
		MT[Si][Ki] = iter + ß;
		MT[Sj][Kj] = iter + ß;
	}
