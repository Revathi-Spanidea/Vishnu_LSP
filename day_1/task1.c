#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#define size 4096

int main(int argc,char*argv[])
{
if(argc!=3)
{
printf("usage :%s <file1> <file2>",argv[0]);
exit(0);
}

int file1=open(argv[1],O_RDONLY);
if(file1==-1)
{
printf("unable to open the file\n");
exit(0);
}
int file2=open(argv[2],O_WRONLY|O_CREAT,0644);
if(file2==-1)
{
printf("unable to create a file\n");
close(file1);
exit(0);
}

char buf[size];

int a,b;

while((a=read(file1,buf,size))>0)
{
b=write(file2,buf,a);
if(a!=b)
{
printf("unable to copy to destination file\n");
close(file1);
close(file2);
exit(0);
}
}

if(a==-1)
{
printf("unable to copy to buffer\n");
close(file1);
close(file2);
exit(0);
}

if((close(file1)||close(file2))==-1)
{
printf("unable to close a file\n");
exit(0);
}

printf("file is copied successfully\n");
}
