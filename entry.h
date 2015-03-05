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

/* objet libre utilitaire */
object *np;

/*
 *
 * Fonction à implémenter / modifier qui dépendent
 * du contenu de l'objet.
 *
 *
 */
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
