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
int main (int argc, char** argv){
	int pip1[2];
	if (pipe(pip1)<0) {printf("can't create pipe1\n"); exit(-1);}
	close(1); //close standart output, now 1 is free
	(void) umask(0); //needed  to create file with appropriate mask
	int fd;//it is going to be our new output stream, because will receive number 1 
	//fd=open("minifile",O_RDWR|O_CREAT,0666);
	//fd=open("minifile",O_RDONLY);	
	fd=open("minifile",O_WRONLY|O_CREAT,0666);
	int son;
	if (son=fork())
		{//case parent
			//close(pip1[0]);//will write only
			int son3;	
					if ((son3=fork())!=0)
						{//case parent
						//printf("here just for fun\n");						
							execl("/bin/ls","ls",argv[1],NULL);//call ls, result will be in file
						}
					else
						{//case second son					
							int sizep;
							int inpipesize;
							char string[100];
							int i;
							pid_t ppid;
							while ((ppid=getppid())!=1) {ppid=0;}//guarantees that file exists and full
							close(fd);
							fd=open("minifile",O_RDONLY);
							while ( (sizep=read(fd,string,100))>0)
								{
									printf("sizep %d \n",sizep);
									//assume blocking is on
									i=0;
								while ((sizep>0)&&(inpipesize=write(pip1[1],string+i,sizep)<sizep))
										{//unsuccesfull writing
								//if (inpipesize<0) {printf("can't inpipe\n"); exit(-1);}//no blocking
											i+=inpipesize;
											sizep-=inpipesize;
										}
									
								}
							//now pipe work is done
							close(fd);
							close(pip1[1]);
							execl("/bin/rm","rm","minifile",NULL);
						}
		}		
	else	//case first son
		{
			close(pip1[1]);//will read only
			char string[101];
			string[100]='\0';
			int size;
			(void)umask(0);
			mknod("minififo",S_IFIFO|0666,0);//creates fifo
			int fifofd=open("minififo",O_WRONLY);//without O_NDELAY flag it is blocked until someone reads it
			int infifo;
			int i=0;
			while ((size=read(pip1[0],string,100))>0)
				{
					string[size]='\0';
					infifo=write(fifofd,string,size);
					i=infifo;
					while ((infifo>0)&&(i<size))
						{
							infifo=write(fifofd,string+i,size);
							i+=infifo;
						}
				}
			close(pip1[0]);
			close(fifofd);
		}
return 0;	//politeness
}
