#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "parentpid.h"

#define N 100

int main (int argn, char * argv[]){
    	system("pidof zombie | awk '{ print \"kill -7 \" $2 }' | sh");
	return 0;
}
