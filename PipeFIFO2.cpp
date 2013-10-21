//PipeFIFO 2 program
#include <stdio.h> //for debugging
#include <unistd.h> //system calls
#include <sys/types.h> //system calls
#include <fcntl.h> //for open specifications
#include <string.h> //to play with string
#include <malloc.h> //to play with string
#include <errno.h>
#include <sys/stat.h>//for umask
#include <stdlib.h>//for exit 
int main (){
	int fd;
	if ((fd=open("minififo",O_RDONLY|O_NDELAY))<0)
		{printf("can't open fifo for reading\n"); exit(-1);}
	char s[100];
	int size;
	int j=0;
	while ((size=read(fd,s,100))!=0)//I know I should have used define
		{
			if (size>0)
			{
			s[size]='\0';
			printf("%s",s);
			}
			else
				{
					j++;
					if (j==10000000)
					{break;}
				}
		}
	close(fd);
	execl("/bin/rm","rm","minififo",NULL);
return 0;	//politeness
}
