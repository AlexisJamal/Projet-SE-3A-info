#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <time.h>

#define N 100

int * ptdr; // Un pointeur drôle
unsigned int semid;
time_t t;
const int VOID_SQ = 2;
const int FIRST_PLAYER = 0;
const int SECOND_PLAYER = 1;

int isFull(){
	for(int i = 1; i < 10; i++){
		if(ptdr[i] == VOID_SQ){
			return 0;
		}
	}
	return 1;
}

void displayGrid() {
	for(int i = 1; i < 10; i++){
		printf("| %d |", ptdr[i]);
		if((i % 3) == 0){
			printf("\n");
		}
	}
}

void fill() {
	for(int i = 1; i < 10; i++){
		ptdr[i] = VOID_SQ;
	} 
}

int checkLine() {
	for(int i = 1; i < 4; i++){
		if(ptdr[i*3] != VOID_SQ && ptdr[i*3] == ptdr[3*i-1] && ptdr[3*i-1] == ptdr[i*3-2]){
			printf("Victoire : ligne\n");
			return 1;
		}
	}
	return 0;
}

int checkColumn() {
	for(int i = 1; i < 4; i++){
		if(ptdr[i] != VOID_SQ && ptdr[i] == ptdr[i+3] && ptdr[i+3] == ptdr[i+6]) {
			printf("Victoire : colonne\n");
			return 1;
		}	
	}
	return 0;
}

int checkDiagTopLeft() {
	if(ptdr[1] != VOID_SQ && ptdr[1] == ptdr[5] && ptdr[5] == ptdr[9]){
		printf("Victoire : diagonale haut gauche\n");
		return 1;
	}
	return 0;
}

int checkDiagTopRight() {
	if(ptdr[3] != VOID_SQ && ptdr[3] == ptdr[5] && ptdr[5] == ptdr[7]){
		printf("Victoire : diagonale haut droite\n");
		return 1;
	}
	return 0;
}

int checkVictory() {
	return (checkLine() == 1) || (checkColumn() == 1) || (checkDiagTopLeft() == 1) || (checkDiagTopRight() == 1);
}

int main (int argn, char * argv[])
{	
	srand((unsigned) time(&t)); 

	pid_t pid; // pour baptiser notre enfant
	pid_t pid2;
	int shmid; // Ah non ! la cuisine c'est moi, et SHMID !

	shmid = shmget(IPC_PRIVATE,N*sizeof(int),IPC_CREAT|0700); // On crée un share memory
	ptdr = shmat(shmid,NULL,0); //void * shmat(int shmit, const void * adr, int option)
	// 2: NULL pour laisser le système trouver tout seul
	// 3: SHM_RDONLY, ou 0 pour permetre Lecture / écriture (par défault )

	ptdr[0] = 0;

	semid = semget(IPC_PRIVATE, 3, 0666|IPC_CREAT);

	semctl(semid, 0, SETVAL, 1); 
	semctl(semid, 1, SETVAL, 0);
	semctl(semid, 2, SETVAL, 0); 

	struct sembuf firstParent = {0, 1, 0}; 
	struct sembuf firstChild1 = {1, 1, 0}; 
	struct sembuf firstChild2 = {2, 1, 0};

	struct sembuf secondParent = {0, -1, 0}; 
	struct sembuf secondChild1 = {1, -1, 0}; 
	struct sembuf secondChild2 = {2, -1, 0}; 

	pid = fork(); // On fait un fork et on récupère le résultat
	pid2 = fork();
	if(pid > 0 && pid2 > 0) 
	{ 
		fill();
		int cmt = 0;
		for(int i = 0; i < 10; i++){
			if(checkVictory() == 0) {
				semop(semid, &secondParent, 1);
				if(cmt == 0) {
					semop(semid, &firstChild1, 1);
					cmt++;
				} else {
					semop(semid, &firstChild2, 1);
					cmt--;
				}
			} else {
				displayGrid();
				return EXIT_SUCCESS;
			}
		}

		ptdr[0] = 1;
		displayGrid();

		semop(semid, &firstChild1, 1);
		semop(semid, &firstChild2, 1);

		wait(NULL);
		wait(NULL);
		
		semctl(semid, 0, IPC_RMID); 
		semctl(semid, 1, IPC_RMID); 
		semctl(semid, 2, IPC_RMID); 

		return EXIT_SUCCESS;
	}
	else 
	{ 
		while(ptdr[0] == 0){
			(pid == 0) ? semop(semid, &secondChild1, 1) : semop(semid, &secondChild2, 1);
			if(ptdr[0] == 0){
				if(!isFull()) {
					int indice = (rand() % 9) + 1;
					if(pid == 0) {
						while(ptdr[indice] == 0){
							indice = (rand() % 9) + 1;
						}
						ptdr[indice] = FIRST_PLAYER;
					} else {
						while(ptdr[indice] != VOID_SQ){
							indice = (rand() % 9) + 1;
						}
						ptdr[indice] = SECOND_PLAYER;
					}
				}
				semop(semid, &firstParent, 1);
			}
		}	
		return EXIT_SUCCESS;
	}
}
