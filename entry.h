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
	/* ne pas éditer */
	pointers	entries;
};

typedef	struct entry object;

typedef struct {
	TAILQ_HEAD(thead, entry) head;
	object *obj;
} ArrayList;

/*
 *
 * Fonction à implémenter / modifier qui dépendent
 * du contenu de l'objet.
 *
 *
 */
object *set(object *,object *);

object *new(unsigned int id,char *);

void toString(object *);

void cleanList(ArrayList *);

void *toArray(ArrayList *);

int isClean(ArrayList *);

void setName(char *, object *);

char *getName(object *);

int cmpId(const void *,const void *);
int cmpName(const void *,const void *);

