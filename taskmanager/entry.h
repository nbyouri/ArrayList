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

#include <kvm.h>
#include <sys/sysctl.h>
struct entry {
        /* index */
        unsigned int    id;
        /* cpu tasks */
        unsigned int    uid;
        char            uid_name[256];
        unsigned int    pid;
        unsigned int    ppid;
        char            name[256];
        char            cmdline[1024];
        char            state[16];
        float           cpu_user;
        float           cpu_system;
        uint64_t        vsz;
        uint64_t        rss;
        short           prio;

        /* ne pas éditer */
        pointers	entries;
};

typedef	struct entry object;

typedef struct {
        struct list_head *head;
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

object *new(unsigned int id);

void toString(object *);

void cleanList(ArrayList *);

void *toArray(ArrayList *);

void setName(char *, object *);

char *getName(object *);

int cmpId(const void *,const void *);
int cmpName(const void *,const void *);

