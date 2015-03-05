# List, une librairie de listes chaînées haut niveau en C

## Utilisation

- Définir la structure de l'objet `object` dans `entry.h`, par exemple:

	```c
	struct entry {
		unsigned int	id;
		char			nom[100];
		char			sexe;
		pointers(entry) entries;
	}
	```
	
- Vous pouvez ajouter autant de champs que vous voulez pour autant que
	vous laissiez le champs `entries` qui contient des pointeurs vers les 
	objets précédents et suivants dans la liste nécessaire pour ajouter, 
	déplacer, supprimer, trier dans la listeet `id` qui permet d'indexer la liste et qui est nécessaire au bon fonctionnement des fonctions décrites après. 


## Fonctions

fonction | description
---------:|:-----------
*foreach*(np) {} | boucle while itèrant de la tête vers la queue
*foreach_rev*(np) {} | boucle while itèrant de la queue vers la tête
*new*(unsigned int id, ...) | constructeur d'objet, doit être défini par l'utilisateur. `new()` 	alloue la mémoire nécessaire pour un objet.
*set*(object *obj, object *new) | remplace l'objet `obj` avec un autre objet `new`.
*add*(object *obj)` | ajouter un objet au bout de la liste.
*addOnTop*(object *obj)` | ajoute un objet en haut de la liste.
*addBefore*(object *obj, object *new)`| ajoute un objet avant un autre.
*addAfter*(object *obj, object *new)` | ajoute un objet après un autre.
*rm*(object *)| retire un objet de la liste.
*get*(unsigned int) |	trouve un objet de la liste basé sur son index.
*getFirst*() | récupère le premier objet de la liste.
*getLast*()  |	récupère le dernier objet de la liste.
*getPrev*(object *obj) |récupère l'élément précédent.
*getNext*(object *obj) |récupère le prochain élément.
*setPrev*(object *obj, object *new)  |remplace l'objet précédent à `obj` avec un nouvel objet `new`.
*setNext*(object *obj, object *new) |remplace l'objet prochain à `obj` avec un nouvel objet `new`.
*getId*(object *obj)    |retourne l'id de l'objet `obj`
*getSize*() |retourne le nombre d'objets dans la liste
*swap*(object *first, object *second)  |échange l'objet `first` avec l'objet `second`
*swapPrev*(object *obj)  |échange l'objet `obj` avec son précédent.
*swapNext*(object *obj)  |échange l'objet `obj` avec son prochain.
*isEmpty*() |retourne 1 si la liste est vide.
*isClean*()   |itère dans la liste pour vérifier que les éléments soient des pointeurs NULL.
*cleanList*()   |vide la liste et nettoie les pointeurs.
*toArray*(char **)|    convertit un champs de l'objet en un tableau à partir des objets de la liste.
*toString*() | affiche l'objet à l'écran de manière lisible pour l'utilisateur.
*sort*() | prend en paramètre une fonction de com paraison d'objets, celle ci doit être définie comme suit: `int cmp(const void *, const void *);`
	
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

## Important

- Ne pas oublier de désallouer l'entrée "token" qui permet d'itérer dans la liste et les autres éventuels objets générés par le constructeur `new()`. 	

## Programme d'exemple

- entry.h

```c
/*
 *
 * Définition de l'objet de la liste chaînée.
 *
 * Vous pouvez ajouter des champs au choix, tant que la structure est bien
 * nommée entry et qu'elle contient les pointeurs.
 *
 * Ne pas oublier d'implémenter les fonctions spécifiques à vos champs
 * pour utiliser avec les fonctions de comparaison pour sort().
 *
 */


struct entry {
	unsigned int	id;
	char		*name;
	/* obligatoire */
	pointers	entries;
};

/* obligatoire */
typedef	struct entry object;

void cleanList();
void toString(object *en);
void *toArray();
void isClean(void);
ssize_t setName(char *name,object *en);
int cmpId(const void *a,const void *b);
int cmpName(const void *a,const void *b);
char *getName(object *en);
object *set(object *base,object *en);
object *new(unsigned int id,char *name);

```

- entry.c

```c
#include <assert.h>
#include "list.h"
#include "tools.h"

/* Fonctions à implémenter */

object *new(unsigned int id, char *name) {
	object *en = NULL;
	en = growArray(en, 1, sizeof(*en));
	en->id = id;
	en->name = growArray(en->name, BUFSIZ, sizeof(*(en->name)));
	if (name != NULL) {
		strlcpy(en->name, name, BUFSIZ);
	}
	return en;
}

object *set(object *base, object *en) {
	assert(en != NULL);
	
	if (base == NULL) {
		base = new(0, "(null)");
	}

	if (en->name != NULL) {
		strlcpy(base->name, en->name, BUFSIZ);
	} else {
		base->name = NULL;
	}

	base->id = en->id;
	return base;
}

char *getName(object *en) {
	return (en->name);
}

int cmpName(const void *a, const void *b) {
	object *en1 = (object *)a;
	object *en2 = (object *)b;
	return (strcasecmp(en1->name, en2->name));
}

int cmpId(const void *a, const void *b) {
	object *en1 = (object *)a;
	object *en2 = (object *)b;
	return ((int)en2->id - (int)en1->id);
}

ssize_t setName(char *name, object *en) {
	assert(en != NULL && name != NULL);
	if (en->name == NULL) {
		en->name = growArray(en->name, BUFSIZ, sizeof(char));
	}
	if (strlen(name) > 0) {
		return (ssize_t)strlcpy(en->name, name, BUFSIZ);
	}
	return -1;
}

void isClean(void) {
	foreach (np) {
		assert(np != NULL);
		assert(np->name != NULL);
	}
}

void *toArray() {
	char **array = NULL;
	unsigned int i = 0;
	array = growArray(array, getSize(), sizeof(char *));
	if (array == NULL) {
		perror("*array == NULL");
	}
	foreach (np) {
		array[i] = growArray(array[i], BUFSIZ, sizeof(char));
		strlcpy(array[i], np->name, BUFSIZ);
		i++;
	}
	return array;
}

void toString(object *en) {
	printf("%02u -> %s\n", en->id, en->name);
}

void cleanList() {
	foreach (np) {
		np->name = cleanPtr((char **)np->name, NULL);
		assert(np->name == NULL);
		rm(np);
	}
}
```

- examples.c

```c
#include <dirent.h>
#include "list.h"
#include "tools.h"

/* Dossier à regarder */
#define PATH "/Users/youri/Downloads"

int main(void) {
	DIR                 *dp;
	struct dirent       *ep;

	/* initialisation de la liste */
	initList();

	/* ouvrir PATH directory */
	if ((dp = opendir(PATH)) == NULL) {
		perror("Failed to opendir");
		return -1;
	} else {
		/* récupérer un listing de dossiers depuis PATH */
		while ((ep = readdir(dp)) != NULL) {
			if ((ep->d_name[0] != '.')) {
				/* add directory in list */
				unsigned int id = random();
				np = new(id, ep->d_name);
				add(np);
			}
		}
		/* fermeture du dossier */
		if (closedir(dp) == -1) {
			perror("Failed to close dir pointer");
			return -1;
		}
	}
	
	/* échange de deux objets basé sur leur index */
	swap(get(29), get(2));
	
	/* affichage du premier objet */
	toString(getFirst());
	
	/* tri sur les noms */
	sort(cmpName);


	/* affichage du contenu */
	foreach (np) {
		toString(np);
	}

	/* vidage & nettoyage de la liste */
	cleanList();

	/* désallocation de l'objet 'token' */
	np = cleanPtr((char **)np, NULL);
	
	/* cassos */
	return 0;
}
```

Compilez avec

- `cc list.c tools.c examples.c -o test`

## Détails d'implémentation

- List est une implémentation haut niveau des listes chaînées en C.
- Elle utiliser les TAILQ de BSD  (voir queue.h)

## Auteur

Youri Mouton, 5 Mars 2015.
