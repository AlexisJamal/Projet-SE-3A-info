# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
int main (void)
{
	struct tube_t {
		int read;
		int write;
	} tube; // équivalent à int[2] mais plus parlant
	
	char buf[18]; // pour recuperer les caracteres lus dans le tube
	int pid;
	
	if ((pipe((int *)&tube) == -1) || (pid=fork()) == -1)
		perror("erreur");
	
	if (pid > 0)
	{// processus pere
		close(tube.read);
		sleep(2); // attente
		write(tube.write,"Je suis ton père" , 18);
		// UTF8 prends plus d'un caractère pour les accents + '\0'
		close(tube.write);
	}
	else
	{// processus fils
		close(tube.write);
		sleep(1); // attente 
		read(tube.read ,buf, 18);
		printf( "[%d] : %s\n", getpid(), buf);
		close(tube.read);
		printf( "Noooooooonnnnn !");
	}
	
	exit(EXIT_SUCCESS);
}
