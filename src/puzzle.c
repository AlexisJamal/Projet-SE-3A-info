#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (){
	unsigned int semid;

	semid = semget(IPC_PRIVATE, 6, 0666|IPC_CREAT|IPC_EXCL);//crée 2 sémaphore
	semctl(semid, 0, SETVAL, 1); //initialise la valeur à 1 sur le premier sémaphore
	semctl(semid, 1, SETVAL, 0); //initialise la valeur à 0 sur le deuxième sémaphore
	semctl(semid, 2, SETVAL, 0);
	semctl(semid, 3, SETVAL, 0);
	semctl(semid, 4, SETVAL, 0);
	semctl(semid, 5, SETVAL, 0);
	
	for (int i = 0; i < 6; i++){
	
		FILE * fp;
		char * line = NULL;
		size_t len = 0;
		char *path;
		asprintf(&path, "data/f%d", i+1);
		fp = fopen(path, "r"); // ouvre le fichier
		if (fp == NULL){
			asprintf(&path, "../data/f%d", i+1);
			fp = fopen(path, "r"); // ouvre le fichier
		}	
		
		if(fp == NULL) {
			printf("bug");
			exit(EXIT_FAILURE); // oups !
		}
		
		else{
			
			if(!fork()) {
				struct sembuf up = {(i+1)%6, 1, 0}; //number, operation, flag
				struct sembuf down = {(i)%6, -1, 0}; //number, operation, flag
				while (getline(&line, &len, fp) != -1) { // Tant qu'on peu lire
			   			
					semop(semid, &down, 1);
		   			printf("%s", line); // on affiche !
		   			fflush(stdout);
		   			semop(semid, &up, 1);
				}
				if (line){
		   			free(line); // et de libérer la mémoire
					exit(EXIT_SUCCESS);
					
				}
			}
			
		}
		fclose(fp);
		
	}	
	wait(NULL);
	sleep(1);
	semctl(semid, 0, IPC_RMID); //ferme le sémaphore
	semctl(semid, 1, IPC_RMID); //ferme le sémaphore
	semctl(semid, 2, IPC_RMID); //ferme le sémaphore
	semctl(semid, 3, IPC_RMID); //ferme le sémaphore
	semctl(semid, 4, IPC_RMID); //ferme le sémaphore
	semctl(semid, 5, IPC_RMID); //ferme le sémaphore
	return 0;
}
