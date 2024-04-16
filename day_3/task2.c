#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<semaphore.h>
#include<sys/wait.h>

void generate(int num[])
{
for(int i=0;i<10;i++)
{
num[i]=i+1;
}
}

int calculate(int num[])
{
int sum=0;
for(int i=0;i<10;i++)
{
sum =sum+num[i];
}
return sum;
}
int main()
{
sem_t *sem=sem_open("semaphore",O_CREAT,0644,1);
if(sem==SEM_FAILED)
{
printf("error");
exit(0);
}
pid_t pid=fork();
if(pid==-1)
{
printf("error");
exit(0);
}
else if(pid==0)
{
printf("child process id is %d:",getpid());
sem_wait(sem);
int num[10];

generate(num);

for(int i=0;i<10;i++)
{
printf("%d ",num[i]);
}

sem_post(sem);

printf("\n");

sem_close(sem);
}
else
{
printf("parent process id is %d:",getpid());

wait(NULL);
sem_wait(sem);
int num[10];
generate(num);

int sum=calculate(num);
printf("%d\n",sum);
sem_post(sem);

sem_close(sem);

sem_unlink("semaphore");
}
}
