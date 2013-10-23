//I probably misunderstood your instructions about style 
//PipeFIFO 1 programm
#include <stdio.h> //for debugging
#include <unistd.h> //system calls
#include <sys/types.h> //system calls
#include <fcntl.h> //for open specifications
#include <string.h> //to play with string
#include <malloc.h> //to play with string
#include <errno.h>
#include <sys/stat.h>//for umask
#include <stdlib.h>//for exit
#define N 100 //buffer capacity, copying from file to pipe[] through string[N]
#define M 100 //string length, copying from pipe[] to fifo via string[M]
int main (int argc, char** argv)
{

int pipefd[2];
if (pipe(pipefd)<0) 
{
	printf("can't create pipe1\n");
	 exit(-1);
}

close(1); //close standart output, now 1 is free
(void) umask(0); //needed  to create file with appropriate mask
int fd=open("minifile",O_WRONLY|O_CREAT,0666);//it is going to be our new output stream, because will receive number 1

if (int son=fork())
{//case parent
	close(pipefd[0]);//will write only
	int son2;
	if ((son2=fork())!=0)
	{//case parent						
		execl("/bin/ls","ls",argv[1],NULL);//call ls, result will be in file
	}
	else
	{//case second son					
		int sizep;
		int inpipesize;
		char string[N];
		int i;
		pid_t ppid;
		while ((ppid=getppid())!=1) //do nothing until parent has finished his execl
		{
			ppid=0;
		}//this cycle guarantees that file exists and full

		close(fd); //now need to read the file, not to write; cursor position must be changed  
		fd=open("minifile",O_RDONLY);

		while ( (sizep=read(fd,string,N))>0)
		{
			i=0;
			while ((sizep>0)&&(inpipesize=write(pipefd[1],string+i,sizep)<sizep))
			{//case of unsuccesfull writing
				i+=inpipesize;
				sizep-=inpipesize;
			}			
		}
			//now pipe work is done
		close(fd);
		close(pipefd[1]);
		execl("/bin/rm","rm","minifile",NULL);
	}
}		
else	//case first son
{
	close(pipefd[1]);//will read only
	char string[M];
	int size;
	(void)umask(0);
	mknod("minififo",S_IFIFO|0666,0);//creates fifo
	int fifofd=open("minififo",O_WRONLY);//without O_NDELAY flag it is blocked until someone reads it
	int infifo;
	int i=0;

	while ((size=read(pipefd[0],string,M))>0)
	{
		infifo=write(fifofd,string,size);
		i=infifo;
		while ((infifo>0)&&(i<size))
		{
			infifo=write(fifofd,string+i,size);
			i+=infifo;
		}
	}

	close(pipefd[0]);
	close(fifofd);
}
return 0;	//politeness
}
