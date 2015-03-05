# List, une librairies haut niveau en C

	- List est une implémentation haut niveau des listes chaînées en C.
	- Elle utiliser les TAILQ de BSD  (voir queue.h)

# Comment utiliser
	- Définir la structure de l'objet dans entry.h, par exemple:

		struct entry {
			char		nom[100];
			char		sexe;
			pointers(entry) entries;
		}
	
	- Vous pouvez ajouter autant de champs que vous voulez pour autant que
	vous laissez le champs 'entries' qui contient des pointeurs vers les 
	objets précédents et suivants dans la liste nécessaire pour ajouter, 
	déplacer, supprimer, trier dans la liste.


# Fonctions possibles

Après avoir initialisé
