#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
pid_t pid;
char *command[]={"date","pwd"};
int i;
for(i=0;i<2;i++)
{
pid=fork();
if(pid==-1)
{
printf("unable to create a process using fork()");
exit(0);
}
if(pid==0)
{
printf("child process %d is %s\n",getpid(),command[i]);
execlp(command[i],command[i],NULL);
printf("error execlp");
exit(0);
}
}
}
