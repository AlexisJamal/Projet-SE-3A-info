#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    //char path[3] = { 'f', '1'+3, 0 };
    char *path;
    asprintf(&path, "data/f%d", 3);

    fp = fopen(path, "r"); // ouvre le fichier
    if (fp == NULL)
	   exit(EXIT_FAILURE); // oups !

    while (getline(&line, &len, fp) != -1) { // Tant qu'on peu lire …
	   printf("%s", line); // on affiche !
	   fflush(stdout);
    }

    fclose(fp); // /!\ Ne pas oublier de fermer …
    if (line)
	   free(line); // et de libérer la mémoire
    exit(EXIT_SUCCESS);
}
