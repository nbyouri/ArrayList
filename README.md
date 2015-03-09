# ArrayList, une librairie de listes chaînées haut niveau en C

## Utilisation

- Définir la structure de l'objet `object` dans `entry.h`, par exemple:

	```c
	struct entry {
		unsigned int	id;
		char			nom[100];
		char			sexe;
		pointers		entries;
	}
	```
	
- Vous pouvez ajouter autant de champs que vous voulez pour autant que
	vous laissiez le champs `entries` qui contient des pointeurs vers les 
	objets précédents et suivants dans la liste nécessaire pour ajouter, 
	déplacer, supprimer, trier dans la liste.     
	`id` permet d'indexer la liste.

- voici un exemple d'utilisation simple: 

	```c
		ArrayList *list;
		/* alloue de la mémoire pour le pointeur list */
		list = malloc(sizeof(*list));
		
		/* initialise la liste */
		initList(list);

		/* ajoute 10 objets dans la liste */
		for (int i = 0; i < 10; i++) {
			list->obj = new(i, "test");
			add(list, list->obj);
		}
		
		/* affiche le contenu de la liste */
		foreach(list) {
			toString(list->obj);
		}
		
		/* détruit les objets et les retire de la liste */
		cleanList(list);
		
		/* désalloue la mémoire pour le pointeur list */
		free(list);
		list = NULL;
	```


## Fonctions

fonction             | description
--------------------:|:-----------
*initList*(ArrayList *) | initialise la liste en allouant de la mémoire pour la tête
*foreach*(ArrayList *) {} | boucle while itèrant de la tête vers la queue
*foreach_rev*(ArrayList *) {} | boucle while itèrant de la queue vers la tête
*new*(unsigned int id, ...) | constructeur d'objet, doit être défini par l'utilisateur. `new()` 	alloue la mémoire nécessaire pour un objet.
*set*(object *obj, object *new) | remplace l'objet `obj` avec un autre objet `new`.
*add*(ArrayList *, object *obj) | ajouter un objet au bout de la liste.
*addOnTop*(ArrayList *, object *obj) | ajoute un objet en haut de la liste.
*addBefore*(ArrayList *, object *obj, object *new)| ajoute un objet avant un autre.
*addAfter*(ArrayList *, object *obj, object *new) | ajoute un objet après un autre.
*rm*(ArrayList *, object *)| retire un objet de la liste.
*get*(ArrayList *, unsigned int) |	trouve un objet de la liste basé sur son index.
*getFirst*(ArrayList *) | récupère le premier objet de la liste.
*getLast*(ArrayList *)  |	récupère le dernier objet de la liste.
*getPrev*(ArrayList *, object *obj) |récupère l'élément précédent.
*getNext*(ArrayList *, object *obj) |récupère le prochain élément.
*setPrev*(ArrayList *, object *obj, object *new)  |remplace l'objet précédent à `obj` avec un nouvel objet `new`.
*setNext*(ArrayList *, object *obj, object *new) |remplace l'objet prochain à `obj` avec un nouvel objet `new`.
*getId*(object *obj)    |retourne l'id de l'objet `obj`
*getSize*() |retourne le nombre d'objets dans la liste
*swap*(object *first, object *second)  |échange l'objet `first` avec l'objet `second`
*swapPrev*(ArrayList *, object *obj)  |échange l'objet `obj` avec son précédent.
*swapNext*(ArrayList *, object *obj)  |échange l'objet `obj` avec son prochain.
*isEmpty*(ArrayList *) |retourne 1 si la liste est vide.
*cleanList*(ArrayList *)   |vide la liste et nettoie les pointeurs.
*toArray*(ArrayList *)|    convertit un champs de l'objet en un tableau à partir des objets de la liste.
*toString*(object *) | affiche l'objet à l'écran de manière lisible pour l'utilisateur.
*sort*(ArrayList *, int(\*)(const void *, const void *)) | prend en paramètre une fonction de comparaison d'objets.
	
### / ! \ Fonctions à modifier dans `entry.c` / ! \

Certaines fonctions dépendent du contenu de l'objet: 

- Le constructeur `new()` pour créer les objets.
- La fonction `set()` pour allouer et initialiser un objet dans la liste.
- Les fonctions `cleanList()` et `isClean()` pour nettoyer les eventuels pointeurs ajoutés dans la définition de l'objet par l'utilisateur.
- `toArrray()`, pour créer un tableau dynamique d'un élément à partir des objets de la liste.
- `toString()`, pour afficher à l'écran des champs de la structure.
- getters et setters éventuels, voir programme d'exemple.

### Fonctions à ajouter dans `entry.c`
- Optionnellement, ajouter des fonctions de tris pour sort().
	Par exemple, pour trier sur un champs `char *name`:

	```c
	 int cmpName(const void *a, const void *b) {
	 	object *en1 = (object *)a;
	 	object *en2 = (object *)b;
	 	return (strcasecmp(en1->name, en2->name));
	 }
	 ...
	sort(cmpName);
	```	

## Programme d'exemple

Modifier la structure de l'objet dans entry.h et ajouter des fonctions au souhait dans entry.c

Compilez l'exemple avec

- `make`
- `cc example.c libList.dylib -o test`

## Détails d'implémentation

- List est une implémentation haut niveau des listes chaînées en C.
- Elle utiliser les TAILQ de BSD  (voir queue.h)

## Auteur

Youri Mouton, 5 Mars 2015.
