#include<stdio.h>
int main()
{
char* exist_var=getenv("EXIST_VAR");
if(exist_var!=0)
{
printf("%s",exist_var);
}
else
{
printf("error");
}
char *new_var="NEW_VAR";
char *new_val="NEW_VAL";
if(setenv(new_var,new_val,1)!=0)
{
printf("error");
return 1;
}
printf("set %s=%s\n",new_var,new_val);
char *new=getenv(new_var);
if(new!=0)
{
printf("value is %s:%s\n",new_var,new_val);
}
else
{
printf("not set");
}
}
