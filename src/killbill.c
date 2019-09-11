#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "parentpid.h"

#define N 100

int main (int argn, char * argv[]){
	int pid = atoi(argv[1]);
	kill(parentpid(pid), SIGBUS);
	return 0;
}
