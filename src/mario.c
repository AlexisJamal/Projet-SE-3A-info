# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (void)
{
	struct tube_t {
		int read;
		int write;
	} tube; // équivalent à int[2] mais plus parlant
	
	int pid;
	
	if ((pipe((int *)&tube) == -1) || (pid=fork()) == -1)
		perror("erreur");
	
	if (pid > 0)
	{// processus pere
		FILE * file = fopen("data/number", "r+");
		if (file == NULL){
			file = fopen("../data/number", "r+");
		}	
		if(file == NULL) {
			printf("bug");
			exit(EXIT_FAILURE);
		}

		char ch;
		while(ch != EOF) {
			ch = (char) fgetc(file);
			if(ch >='0' && ch <= '9') {
				write(tube.write, &ch , 1);
			}
		}
		write(tube.write, "\0", 1);
		close(tube.write);
		close(tube.read);
		fclose(file);
		wait(NULL);
	}
	else
	{// processus fils
		close(tube.write); 
		dup2(tube.read, 0);
		close(tube.read);

		FILE * test = fopen("bin/self", "r+");
		if(test == NULL) {
			execl("../bin/self", "self", NULL);
		}
		else execl("bin/self", "self", NULL);
		fclose(test);
	}
	
	exit(EXIT_SUCCESS);
}
