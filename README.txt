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
			f(i,j) = 1 si (i,j) ? E, i,j ? V
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
	  - N1(s) = { <Sj, Vi,Vj> | Sj ? Vi n C(s), i != j, |Vk[i]| > floor(|V|/nbColor),
	    |Vk[j]| < top(|V|/nbColor) } o� C(s) repr�sente l'ensemble des sommets en 
	    conflits de la solution courante s.
	  
	  - N2(s) = { (Si,Sj) | Si ? Vk[i], Sj ? Vk[j], i != j, {Si,Sj} n C(s) != {}  }
	  
	Plusieurs choix s'offrent � nous pour la repr�sentation de ces deux voisinages. 
	En effet, nous pouvons � la fois d�finir une classe abstraite N puis deux sous-classes
	N1 et N2 impl�mentant chacun un voisinage puis travailler sur un ensemble d'objets de 
	type N. 
	Nous pourrions �galement travailleur sur deux vecteurs diff�rents repr�sentant chacun
	un voisinage.
	De nombreuses autres impl�mentations sont possibles et nous reviendrons dessus si le 
	temps nous le permet.	
	De plus, la recherche tabou pr�sent�e utilisant une strat�gie de s�lection du meilleur
	am�liorant,nous ajouterons � chacun des voisins le score de gain qui lui est associ�.
	Nous pr�senterons le calcul de ce gain dans la prochaine section.
	Ainsi, nous disposons d'une classe "m�re" Voisin, disposant d'un gain et de deux classes
	"filles" impl�mentant les voisinages OneMove et Swap. Nous n'utiliserons donc qu'un
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
	      
	    vector<Voisin *> N = N1 Union N2;
	  
	Calcul du gain
	--------------
	Afin de s�lectionner le meilleur am�liorant, nous avons mis en place une proc�dure 
	d'�valuation d'un voisin v d�pendant du type de mouvement choisi. Cette proc�dure
	est bas�e sur la matrice  M d�finie pr�cedemment.
	
	- OneMove : delta(<s,i,j>) : M[s][Vk[j]] - M[s][Vk[i]]
	
	- Swap : delta(<u,v,ku,kv>) : M[v][ku] - M[v][kv] + (M[u][kv] - M[u][ku] - 2*G[u][v])
		
	Mise � jour de la coloration apr�s un mouvement
	-----------------------------------
	Apr�s avoir s�lectionn� un mouvement autoris�, deux op�rations doivent �tre effectu�es :
	la mise � jour de M et la mise � jour de Vk. Une fonction de validation par type de 
	voisinage a donc �t� cr��e.
	
	validMovement(Voisin *N){
	    // dynamic_cast et appel � la fonction correspondante
	}
	
	validOneMove(OneMove *om){
	    // MaJ M
	    for(i=0; i < |V|; ++i){
		if( G[om->getS()][i] == 1 ){ // sommets voisins
		    M[i][om->getVki()] -= 1;
		    M[i][om->getVkj()] += 1;
		}
	    }
	    
	    // MaJ Vk
	    Vk[om->getVki()].erase( remove(Vk[om->getVki()].begin(), Vk[om->getVki()].end(), om->getS()), Vk[om->getVki()].end() );
	    Vk[om->getVkj()].push_back(om->getS());
    
	}
	
	validSwap(Swap *s){
	    // M�J de M --> correspond � deux OneMove cons�cutifs
	    for(int i=0; i < |V|; ++i){
		// Les sommets s et i sont voisins
		if( G[s->getSi()][i] == 1){
		    M[i][s->getKi()] -= 1;
		    M[i][s->getKj()] += 1;
		}
	    
		if(G->getMatriceValue(s->getSj(),i) == 1){
		    M[i][s->getKj()] -= 1;
		    M[i][s->getKi()] += 1;
		}
	    }
    
	    // Mise � jour de Vk_
	    Vk[s->getKi()].erase( remove(Vk[s->getKi()].begin(), Vk[s->getKi()].end(), s->getSi()), Vk[s->getKi()].end() );
	    Vk[s->getKj()].push_back(s->getSi());
	    
	    Vk[s->getKj()].erase( remove(Vk[s->getKj()].begin(), Vk[s->getKj()].end(), s->getSj()), Vk[s->getKj()].end() );
	    Vk[s->getKi()].push_back(s->getSj());

	}
	
	
	Gestion de la liste tabou
	-------------------------
	Une fois un mouvement de voisinage effectu� et la matrice M mise � jour, il convient
	d'interdire au(x) sommet(s) ayant chang� de classe de couleur d'y revenir avant un 
	certain nombre d'it�rations via la mise en place d'une matrice tabou.
	De fait, si un voisin OneMove <S,Vi,Vj> est s�lectionn�, le mouvement suivant est 
	interdits :
		- (S,Vi)
	Si le voisin appliqu� est un Swap <Si,Sj,Ki,Kj>, les mouvements suivants sont interdits :
		- (Si,Ki)
		- (Sj,Kj)
		
	Ainsi, cette matrice tabou de taille |V|*k est telle que MT[u][k] repr�sente l'it�ration
	jusqu'� laquelle le mouvement consistant � d�placer le sommet u vers la couleur k a �t�
	ou sera interdit. Nous devons donc d�finir une fonction pour v�rifier qu'un mouvement 
	n'est pas interdit et une fonction pour mettre � jour l'it�ration jusqu'� laquelle
	sera interdit un mouvement apr�s son application. Soit � le nombre d'it�rations pendant
	lequel un mouvement est interdit apr�s application.
	
	isForbidden(Voisin *N, int iter){
	    // dynamic_cast et appel � la fonction correspondante
	}
	
	isForbiddenSwap(Swap * s, int iter){
	    return ( MT[s->getSi()][s->getKj()] > iter || MT[s->getSj()][s->getKi()] > iter );
	}
	
	isForbiddenOneMove(OneMove *om, int iter){
	    return ( MT[om->getS()][om->getVkj()] > iter );
	}
		
	La mise � jour de la matrice MT s'effectue dans le corps de la fonction run() de notre
	recherche tabou. Une fois le voisin selectionn� et valid�, on met � jour tabuMat via un
	dynamic_cast sur le voisin choisi :
	
	try{
	    if( dynamic_cast<Swap*>(N[indN]) == 0 ){
		// OneMove
		OneMove * om = dynamic_cast<OneMove*>(N[indN]);
		tabuMat[om->getS()][om->getVki()] = iteration + tabuTenure;
		current.validOneMove(om);
		
	    }else{
		Swap* s = dynamic_cast<Swap*>(N[indN]);
		tabuMat[s->getSi()][s->getKi()] = iteration + tabuTenure;
		tabuMat[s->getSj()][s->getKj()] = iteration + tabuTenure;
		current.validSwap(s);
	    }
	}catch(exception &e){
	     cerr << "Exception: " << e.what();
	     exit(EXIT_FAILURE);
	}
	
	
	Aspiration
	----------
	    L'aspiration consiste lorsqu'un mouvement est interdit par la matrice Tabu a le selectionn�
	quand m�me s'il conduit � une solution meilleure que la meilleure actuellement d�termin�e.
	Pour faire cette v�rification de mani�re peu co�teuse, nous utiliserons l'algorithme suivant.
	Litt�ralement, nous v�rifions pour chaque sommet s'il est voisin avec le sommet impliqu� dans le
	mouvement. Si oui, nous v�rifions les �ventuelles correspondances entre couleur et modifions
	le r�sultat en cons�quence. Par exemple, pour un voisin de type OneMove :
	
	    int result = 0;
	    for(int i=0; i < nbColor; ++i){
		for(unsigned j=0; j < Vk[i].size(); ++j){
		    
		    if( (G->getMatriceValue(Vk[i][j],om->getS()) == 1) && (i == om->getVkj()) ){
			// Pas besoin de test car on ferait (M[.][.] + 1) != 0 --> Tautologie car M[][] >=0
			++result;
		    }
		    else if( (G->getMatriceValue(Vk[i][j],om->getS()) == 1) && (i == om->getVki()) ){
			result += ( (M[Vk[i][j]][i]-1) != 0 ); 
		    }
		    else{
			result += ( M[Vk[i][j]][i] != 0);
		    }   
		}
	    }
	    return result;
	    
	Gestion de la tabu tenure et de la profondeur de la recherche
	-------------------------------------------------------------