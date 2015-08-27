#include <stdlib.h>
#include <stdio.h>
int main()
{
int n,m,z;
printf("Please Enter the value for the number of ducks to be Inserted");
scanf("%d",&n);
printf("Please Enter the value for the order in which ducks to be deleted");
scanf("%d",&m);
init_circ_list(p_L);
for(i = 1; i<= n; i++)
{

circ_append_duck(list *p_L, n);
}
nextduck = getnextduck(p_L , NULL);

while(p_L != empty)
{
for(i = 1; i<= m; i++)
{
getnextduck(p_L, nextduck);m 
