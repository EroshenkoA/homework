//RWExec first programm
#include <stdio.h> //for debugging
#include <unistd.h> //system calls
#include <sys/types.h> //system calls
#include <fcntl.h> //for open specifications
#include <string.h> //to play with string
#include <malloc.h> //to play with string
#include <errno.h>
/////////////!!!!arguments in format "/home/a" 
int main (int argc, char** argv){

	close(1); //close standart output, now 1 is free
	(void) umask(0); //needed  to create file with appropriate mask
	int fd;//it is going to be our new output stream, because will receive number 1 
	fd=open("List of files:",O_RDWR|O_CREAT,0666);
	execl("/bin/ls","ls",argv[1],NULL);//just call ls
return 0;	//politeness
}
