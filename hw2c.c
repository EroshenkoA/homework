//RWExec second programm
//"write" realization
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
int main(){
        int fd;
        if ((fd=open("List of files:",O_RDWR))<0)
        {printf("error while opening file\n"); exit(-1);}
	char s[100];
	int size;
	while ((size=read(fd,s,100))>0)
		{
			s[size]='\0';
			printf("%s",s);
		}
		close(fd);
                execl("/bin/rm","rm","List of files:",NULL);
return 0;
}
