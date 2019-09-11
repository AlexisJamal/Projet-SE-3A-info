# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <sys/wait.h>

int main (void) {
	struct tube_t {
		int read;
		int write;
	} tube; // équivalent à int[2] mais plus parlant
	

	int pid;
	
	if ((pipe((int *)&tube) == -1) || (pid=fork()) == -1) {
		perror("erreur");
	}
	
	if (pid > 0) {
		wait(NULL);
		dup2(tube.read,0);
		close(tube.write);
		execlp("wc", "wc", "-l", NULL);
	}
	else {
		dup2(tube.write,1);
		close(tube.read);
		execlp("ps", "ps", "-ef", NULL);
	}
	
	exit(EXIT_SUCCESS);
}
