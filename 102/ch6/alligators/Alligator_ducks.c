#include <stdio.h> 
#include <stdlib.h> 
#include "list.h" 
#include "globals.h"
#include "ducksinterface.h"
status write_node( generic_ptr n)
{
printf(" %d ", *(int * )n ) ;
return OK ;
}

int main( int argc , char *argv[] )
{
int n , m , i; 
list ducks , tempduck , nextduck = NULL ; 
printf("AlligatorProblem: Values for N and M ") ; 
scanf("%d %d",&n ,&m ); 

init_circ_list(&ducks); 
for( i = 1 ;  i <= n ; i++) 
circ_append_duck(&ducks , i ) ; 

nextduck = getnextduck(ducks , NULL ) ; 

printf("The ducks eaten by the Alligator in the order of:") ;
 
while(!empty_circ_list(ducks)){ 
for( i = 1 ; i < m ; i++ ) 
nextduck = getnextduck( ducks , nextduck ) ; 
tempduck = nextduck ; 
nextduck = getnextduck(ducks , nextduck) ; 
write_node( DATA(nextduck)) ; 
circ_delete_node(&ducks , nextduck) ; 
nextduck = tempduck ; 
} 
return 0 ; 
}
