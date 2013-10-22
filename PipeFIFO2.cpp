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
#define N 100 //buffer length 
int main ()
{
int fd;
if ((fd=open("minififo",O_RDONLY|O_NDELAY))<0)
	{
		printf("can't open fifo for reading\n"); 
		exit(-1);
	}
char s[N+1];
int size;
int j=0; //-1 branch counter
while ((size=read(fd,s,N))!=0)
	{
		if (size>0)
		{
			s[size]='\0';
			printf("%s",s);
		}
		else
		{
			j++;
			/*if (j==10000000)
			{break;}*/
			//printf("j=%d\n",j);
		}
	}
close(fd);
execl("/bin/rm","rm","minififo",NULL);
return 0;	//politeness
}
