#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

char buf1[]="abcdefghij";
char buf2[]="ABCDEFGHIJ";
f_attrib = S_IRWXU | S_IRWXG | S_IRWXO; //Set permission for the not exist destination file

int main(void){
	int fd=open("3.txt", O_WRONLY | O_CREAT, f_attrib);
	
	if (write(fd,buf1,10)!=10){
		printf("buf1 write error\n");
		exit(1);
	}
	if (lseek(fd,40,SEEK_SET)==-1){
		printf("lseek error\n");
		exit(1);
	}
	if (write(fd,buf2,10)!=10){
		printf("buf2 write error\n");
		exit(1);
	}
	exit(0);
}
