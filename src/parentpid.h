#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
int kill(pid_t pid, int sig);

#define MAXPIDLEN 10
#define MAXPROCPATHLEN (MAXPIDLEN + 11)

//http://www.linuxquestions.org/questions/programming-9/function-for-getting-pid-of-any-process-276839/

pid_t parentpid(pid_t pid)
{
	pid_t ppid;
	char procpath[MAXPROCPATHLEN];
	FILE *procstat;

	snprintf(procpath, MAXPROCPATHLEN, "/proc/%u/stat", pid);
	procstat = fopen(procpath, "r");
	if (procstat < (FILE*)0) {
		perror("fopen(procstat)");
		return 0;
	}
	fscanf(procstat, "%*d %*s %*c %u", &ppid);
	fclose(procstat);
	return ppid;
}


