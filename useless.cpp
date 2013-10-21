//fairly complicated realization, not enough comments, because
//I made a lot of brief changes in code to adapt it to new program
/////////////////////////binary heap first
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
using namespace std;
#define left(i) 2*(i) //A[1] is the first element, not A[0] !!
#define right(i) 2*(i)+1
#define parent(i) (i/2) // parent of 1 is 0
//#define N 100 //size of array
#define min(a,b) ((a)<(b))?(a):(b)
//all elements are > 0 for convenience
struct bin_heap
{
int *A;
char **string;
int last;
int N;
};
bin_heap* create_heap(){
bin_heap* H;
H=(bin_heap*) malloc(sizeof(bin_heap));
H->N=100;
int* A = (int*) calloc (H->N, sizeof(int));
char** string = (char**) calloc (H->N, sizeof(char*));
int i;
for (i=0; i<H->N; i++)
{A[i]=0; string[i]=NULL;}
H->A=A;
H->last=0;
H->string=string;
return H;
}
void strain_up (int i, int*& A, char**& string){
int p;
int buf;
char* strbuf;
while ((p=parent(i)))
{
if (A[p]<=A[i])
{
return;
}
else //must exchange
{
strbuf=string[p];
string[p]=string[i];
string[i]=strbuf;
buf=A[p];
A[p]=A[i];
A[i]=buf;
i=p;
}
}
}
void strain_down (int*& A, char**& string) {
int i=1;
int a,b;
int buf;
char* strbuf;
int min;
while ( A[(a=left(i))] )
{
b=right(i);
int min=min(A[a],(A[b]==0)?A[a]:A[b]);
if (A[a]!=min) a=b;
if (A[i]>min)
{
strbuf=string[i];
string[i]=string[a];
string[a]=strbuf;
buf=A[i];
A[i]=A[a];
A[a]=buf;
i=a;
}
else
{
return;
}
}
}
void push(char* s, int key, bin_heap*& H){
bin_heap BiH=*H;/////////////////////////////////
if (H->last>=H->N-1)
{
H->A=(int*) realloc ((void*)(H->A),(H->last)*2*sizeof(int));
H->string=(char**) realloc((void*)(H->string),(H->last)*2*sizeof(char*));
H->N=2*(H->N);
//cout<<"heap is full, small array size"<<endl;
//return;
}
((*H).last)+=1;
(H->A)[H->last]=key;
//(H->string)[H->last]=(char*) malloc(sizeof(s));
//strcpy((H->string)[H->last],s);
(H->string)[H->last]=s;
BiH=*H;/////////////////////////////////////
strain_up(H->last,H->A,H->string);
}
int siz(bin_heap* h){
return h->last;
}
struct elem
{int priority; char* string;};
elem pop(bin_heap*& H){
// is_empty must be used before pop
int buf;
char* strbuf;
buf=(H->A)[1];
strbuf=(H->string)[1];
if (H->last==1)
{
(H->A)[1]=0;
(H->string)[1]=NULL;
H->last=0;
}
else
{
(H->A)[1]=(H->A)[H->last];
(H->string)[1]=(H->string)[H->last];
(H->A)[H->last]=0;
//(H->string)[H->last]=NULL;
(H->last)--;
strain_down((H->A),(H->string));
}
elem firstelem;
firstelem.priority=buf;
firstelem.string=strbuf;
return firstelem;
}
void print(int* A,int i,int n){
if (A[i]==0) return;
int j;
print(A,right(i),n+1);
for (j=0; j<n; j++)
{
cout<<" ";
}
cout<<A[i]<<endl;
print(A,left(i),n+1);
}
/////////////////////////////////////////////now body
void readpriority(int fd,char* str,int *istr, int* A, int iA, int *Asize, int *size, int n)
{
if ((*size)<0) return;
while (((*istr)<(*size))&&(str[*istr]<='9')&&(str[*istr]>='0'))
{
n=10*n+str[*istr]-48;
(*istr)++;
}
if ((*istr)>=(*size))
{
(*size)=read(fd,str,100);
(*istr=0);
readpriority(fd,str,istr,A,iA,Asize,size,n);
}
else
{
if ((iA)>=(*Asize))
{
(*Asize)*=2;
A=(int*) realloc((void*)A,(*Asize)*sizeof(int));
}
A[iA]=n;
}
}
int signs(int n)
{
int ret=1;
while ((n=n/10)>0)
{
ret++;
} 
return ret;
}
int pow(int base,int degree)
{
int n=1;
for (int i=0; i<degree; i++)
{
n*=base;
}
return n;
}
int readstring(int fd,char* s, int * i, char* p, int *psize,int *size,int ind)
{
while (((*i)<(*size))&&(s[(*i)]!='\n'))
{
if (ind<(*psize))
{
p[ind]=s[(*i)];
ind++;
(*i)++;
}
else
{
(*psize)*=2;
p=(char*) realloc ((void*) p, (*psize)*sizeof(char));
}
}
if ((*i)==(*size))
{
(*size)=read(fd,s,100);
(*i)=0;
readstring(fd,s,i,p,psize,size,ind); 
}
else//s[i]=='\n'
{
(*i)++;
if (ind<(*psize))
{
p[ind]='\0';
//ind++;
//(*i)++;
}
else
{
(*psize)*=2;
//(*i)++;
p=(char*) realloc ((void*) p, (*psize)*sizeof(char));
p[ind]='\0';
}
}
}
char* getname(char* s)
{
	int i=0;
	char* ret;
	while ((s[i]!=' ')&&(s[i]!='\0'))
		{
			i++;
		}
	ret=(char*) malloc ((i+1)*sizeof(char));
	for (int j=0; j<i; j++)
		{
			ret[j]=s[j];
		}
	ret[i]='\0';
return ret;
}
void fillmas(char* s, char** mas, int* massize)
{
	int i=0;
	int j=0;
	int k=0;
	int ind=0;
	char* p;
	
	while (s[i+j]!='\0')
	{	
		while ((s[i+j]!=' ')&&(s[i+j]!='\0'))
			{//read word
				j++;
			}
		p=(char*) malloc ((j+1)*sizeof(char));
		for (k=0; k<j; k++)
		{
			p[k]=s[i+k];
		}		
		p[j]='\0';	
		i+=j+1;
		j=0;
		if (ind<(*massize))
			{
				mas[ind]=p;
				ind++;
			}
		else
			{
				(*massize)*=2;
				mas=(char**) realloc ((void*)mas,(*massize)*sizeof(char*));
				mas[ind]=p;
				ind++;				
			}		
	}	
}
void go(int p, char* s,int prev)
{		
	char** mas=(char**) malloc (5*sizeof(char*));
	int massize=5;
	fillmas(s,mas,&massize);
	if (p>prev) {
		sleep(((double)(p-prev))/2);
		printf("in %f seconds '%s' will be processed:\n",((double)(p-prev))/2,getname(s));
		printf("all arguments are '%s'\n",s);
		sleep(((double)(p-prev))/2);
		}
	int k=fork();
	if (k)
	{execv(getname(s),mas);}
	else return;
}
int main(int argc,char** argv){
bin_heap* H=create_heap();
int fd;
//uselessfile by default;
char name[]="uselessfile";
if (argc==1)
{ //no arguments, use default file
if((fd = open(name, O_RDONLY)) < 0){
printf("Can\'t open file\n"); exit(-1);}
}
else
{//use argument as filename
	if((fd = open(argv[1], O_RDONLY)) < 0){
printf("Can\'t open file\n"); exit(-1);}
}
int i;
char s[100];
int *A;
char **str;
A=(int*) malloc (100*sizeof(int));
int Asize=100;
int strsize=100;
str=(char**) malloc (100*sizeof(char*));
//size_t size;
int size;
i=0;
int c;
int iA=0;
int istr=0;
char* p;
int psize;
///////////////////main cycle
size=read(fd,s,100);//actually I know that it's better to use 'N' against '100'..
while ((size-i)>0)
{//processing file information
readpriority(fd,s,&i,A,iA,&Asize,&size,0);
i++;
psize=40;
p=(char*) malloc (40*sizeof(char));
readstring(fd,s,&i,p,&psize,&size,0);
if (strsize<Asize) {strsize=Asize; str=(char**) realloc ((void*) str, strsize*sizeof(char*));}
str[iA]=p;
iA++;
}
//////////////////////////
if (size<0) {printf("error, can't read\n"); exit(-1);}
close(fd);

for (i=0; i<iA; i++)
{
	push(str[i],A[i],H);
}//use heap to sort files
elem T;
int prev=0;
for (i=0; i<iA; i++)
{	
	T=pop(H);
	printf("\n\n");
	fflush(stdout);
	go(T.priority,T.string,prev);
	prev=T.priority;
}

return 0;
}
