#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void){
	pid_t t;

	printf("Original progam, pid=%d\n",getpid());
	t = fork();
	if (t==0){
		printf("In child process, pid=%d, ppid=%d\n", getpid(), getppid());
	}
	else{
		printf("In parent, pid=%d, fork returned=%d\n", getpid(), t);
	}
}
