/**************************Radix Sort Main Function***********/
/*************************************************************/
/*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "list.h"
#include "list.c"
int main(int argc,char *argv[])
 {
  int A[500000];
  list Bins[10];
  int i,j,k,n,digit;
  n = atoi(argv[1]);
  for(i = 0; i < n; i++)
  {
   A[i] = rand() %  10000;
  }
  printf("\n");
  for(i = 0; i < 20; i++)
  printf("%d", A[i]);
  printf("\n");
  for(i = 0; i < 10; i++)
  init_list(&Bins[i]);
  for(i = 1; i <= 4; i++)
   {
    printf("pass %d:", i);
    for(j = 0; j < n; j++)
     {
      digit = getdigit(A[j], i-1);
      appendanumber(&Bins[digit],A[j]);
     }
    refill(A,Bins);
    for(k = 0; k < 20; k++)
    printf("%d", A[k]);
    printf("\n");
   }
  return 0;
}

int getdigit(int number, int position)
{
 int digit = 1;
 {
  int num = number;
  while ( num /10)
  ++digit;
 }
 digit -= position - 1;
 while(--digit)
 number = number / 10;
 number = number % 10;
 if(number > 0)
 return number;
 else
 return -number;
}

void refill(int A[], list Bins[])
{
 int i;
 int j;
 int n;
 for(i = 0; i < 10; i++)
 {
 while(!empty_list(Bins[i]))
  {
  for(j = 0; j < n; j++)
   {
   int digit = getdigit(A[j], i-1);
   appendanumber(&Bins[digit],A[j]);
   if(empty_list(Bins[i]))
   insert(&Bins[digit],A[j]);
   }
  }
 }
} 

