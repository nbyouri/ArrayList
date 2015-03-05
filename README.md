# List, une librairies haut niveau en C

- List est une implémentation haut niveau des listes chaînées en C.
- Elle utiliser les TAILQ de BSD  (voir queue.h)

## Utilisation

- Définir la structure de l'objet `object` dans entry.h, par exemple:

		struct entry {
			unsigned int id;
			char			nom[100];
			char			sexe;
			pointers(entry) entries;
		}
	
- Vous pouvez ajouter autant de champs que vous voulez pour autant que
	vous laissiez le champs `entries` qui contient des pointeurs vers les 
	objets précédents et suivants dans la liste nécessaire pour ajouter, 
	déplacer, supprimer, trier dans la listeet `id` qui permet d'indexer la liste et qui est nécessaire au bon fonctionnement des fonctions décrites après. 


## Fonctions

- Les fonctions suivantes permettent d'ajouter ou de modifier des objets:
	- `new(unsigned int id, /* autre champs du struct entry */)`    
	constructeur d'objet, doit être défini par l'utilisateur. `new()` 	alloue la mémoire nécessaire pour un objet.
	- `set(object *obj, object *new)`    
	remplace l'objet `obj` avec un autre objet `new`.
	- `add(object *obj)`    
	ajouter un objet au bout de la liste.
	- `addOnTop(object *obj)`    
	ajoute un objet en haut de la liste.
	- `addBefore(object *obj, object *new)`    
	ajoute un objet avant un autre.
	- `addAfter(object *obj, object *new)`    
	ajoute un objet après un autre.
	- `rm(object *)`    
	retire un objet de la liste.
	- `get(unsigned int)`    
	trouve un objet de la liste basé sur son index.
	- `getFirst()`     
	récupère le premier objet de la liste.
	- `getLast()`     
	récupère le dernier objet de la liste.
	- `getPrev(object *obj)`    
	récupère l'élément précédent.
	- `getNext(object *obj)`    
	récupère le prochain élément.
	- `setPrev(object *obj, object *new)`    
	remplace l'objet précédent à `obj` avec un nouvel objet `new`.
	- `setNext(object *obj, object *new)`    
	remplace l'objet prochain à `obj` avec un nouvel objet `new`.
	- `getId(object *obj)`    
	retourne l'id de l'objet `obj`
	- `getSize()`    
	retourne le nombre d'objets dans la liste
	- `swap(object *first, object *second)`    
	échange l'objet `first` avec l'objet `second`
	- `swapPrev(object *obj)`    
	échange l'objet `obj` avec son précédent.
	- `swapNext(object *obj)`     
	échange l'objet `obj` avec son prochain.
	- `isEmpty()`    
	retourne 1 si la liste est vide.
	- `isClean()`     
	itère dans la liste pour vérifier que les éléments soient des pointeurs NULL.
	- `cleanList()`    
	vide la liste et nettoie les pointeurs.
	- `toArray(char **)`    
	convertit un champs de l'objet en un tableau à partir des objets de la liste.
	- `toString()`    
	affiche l'objet à l'écran de manière lisible pour l'utilisateur.
	- `sort()`    
	prend en paramètre une fonction de com paraison d'objets, celle ci doit être définie comme suit:     
	
		>	int cmp(const void *, const void *);
	
	### Fonctions à modifier
	- Le constructeur `new` pour créer les objets.
	- Les fonctions `cleanList()` et `isClean()` pour nettoyer les eventuels pointeurs ajoutés dans la définition de l'objet par l'utilisateur.
	- `toArrray()`, pour créer un tableau dynamique d'un élément à partir des objets de la liste.
	- `toString()`, pour afficher à l'écran des champs de la structure.

	### Fonctions à ajouter
	- Les fonctions de tris pour sort(), par exemple pour trier sur un champs `char *name`:

		> 		int cmpName(const void *a, const void *b) {
		> 			object *en1 = (object *)a;
		> 			object *en2 = (object *)b;
		> 			return (strcasecmp(en1->name, en2->name));
		> 		}
		> 		...
		>		sort(cmpName);