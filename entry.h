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
	pointers(entry)	entries;
};

