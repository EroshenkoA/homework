//RWExec, second programm
//cat realization
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main(){
	int fd;
	if ((fd=open("List of files:",O_RDWR))<0)
	{printf("error while opening file\n"); exit(-1);}
	if (fork()==0){
		while (getppid()!=1){//wait till father process is dead
		printf(" ");}
		execl("/bin/rm","rm","List of files:",NULL);
		}else{
			execl("/bin/cat","cat","List of files:",NULL);
			close(fd);	
		}
return 0;
}
