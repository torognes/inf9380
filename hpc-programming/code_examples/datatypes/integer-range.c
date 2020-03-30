#include <stdio.h>

int main(  int argc, char *argv[] )
{
  char n=2;
  //short n=32;
  //int n=32;
  //long n=32;
  //long long n=32;

  printf("Variable size in bytes %ld\n",sizeof(n));
  
  while (n>0) {
    printf("%d",n);  /* Use %ld for long int */  
    n=n*2;
    printf("  %d\n",n); /* Use %ld for long int */  
  }

}
