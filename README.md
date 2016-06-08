#Equitable Coloring Problem using BITS algorithm

ECP est un algorithme de recherche locale ayant	pour but de résoudre le problème de coloration équitable sur différents graphes
présents dans la littérature.								
Dans un premier temps, nous implémenterons l'algorithme décrit dans	l'article "Backtracking Based Iterated Tabu Search for
Equitable Coloring" disponible à l'adresse :					
http://www.info.univ-angers.fr/pub/hao/papers/LaiHaoGloverEAAI2015.pdf  

Réalisé au cours du travail d'étude et de recherche de 1ère année de Master, cette implémentation parvient à égaler une grande
partie des résultats de l'implémentation originale. Toutefois, la structure de données servant pour le voisinage n'étant pas
optimale, les résultats sont moins bons sur des instances de grandes tailles avec un nombre de couleurs important.

Quelques améliorations ont été apportées à cet algorithme, particulièrement au niveau de divers paramètres initialement fixés de
manière empirique. On pourra notamment citer la probabilité d'appliquer une perturbation aléatoire, le nombre de mouvements
effectués par perturbation et l'application des différentes règles de tabu tenure.
