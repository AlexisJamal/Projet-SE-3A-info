#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define N 100

int main (int argn, char * argv[])
{
	pid_t pid; // pour baptiser notre enfant
	int * ptdr; // Un pointeur drôle
	int shmid; // Ah non ! la cuisine c'est moi, et SHMID !

	shmid = shmget(IPC_PRIVATE,N*sizeof(int),IPC_CREAT|0700); // On crée un share memory
	ptdr = shmat(shmid,NULL,0); //void * shmat(int shmit, const void * adr, int option)
	// 2: NULL pour laisser le système trouver tout seul
	// 3: SHM_RDONLY, ou 0 pour permetre Lecture / écriture (par défault )
	pid=fork(); // On fait un fork et on récupère le résultat

	if(pid > 0)
	{ //processus père
		int indice = (rand()+1)%(N/sizeof(int)); // On calcule un indice au hasard
		ptdr[0]=indice; // On le stocke
		ptdr[indice]=42; // On met une valeur importante dans le tableau
		wait(NULL);
		shmctl(shmid,IPC_RMID,NULL); // /!\ Toujours fermer
	}
	else
	{ //processus fils
		printf("\nptdr=%d\n\n",ptdr[ptdr[0]]);
	}
	return EXIT_SUCCESS;
}
