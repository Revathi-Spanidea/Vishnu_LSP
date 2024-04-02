#include<pthread.h>
#include<stdio.h>
pthread_t a[2];
pthread_mutex_t lock;
int count=0;
unsigned int i;
void *fun(void *arg)
{

pthread_mutex_lock(&lock);
count++;
printf("count is %d",count);
for(i=0;i<2;i++);
printf("count is %d",count);

pthread_mutex_unlock(&lock);
}

int main()
{
if(pthread_mutex_init(&lock,NULL)!=0)
{
printf("mutex is cannot be created\n");
}

while(i<2)
{
int error;
error=pthread_create(&a[i],NULL,&fun,NULL);
i++;
if(error!=0)
{
printf("error");
}
}
pthread_join(a[0],NULL);
pthread_join(a[1],NULL);

pthread_mutex_destroy(&lock);
return 0;
}
