#include <stdio.h>
#include <time.h>

int main(  int argc, char *argv[] )
{
  char a;
  short h;
  int  i;
  long j;
  long long k;
  float x;
  double y;
  long double z;
  time_t t;

  printf(" sizeof char %d\n",sizeof(a));
  printf(" sizeof short %d\n",sizeof(h));
  printf(" sizeof int %d\n",sizeof(i));
  printf(" sizeof long %d\n",sizeof(j));  
  printf(" sizeof long long %d\n",sizeof(k));
  printf(" sizeof float %d\n",sizeof(x));
  printf(" sizeof double %d\n",sizeof(y));
  printf(" sizeof long double %d\n",sizeof(z));
  printf(" sizeof time_t %d\n",sizeof(t));
}
