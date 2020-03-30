#include <stdio.h>
main()
{
  long n=200000000,i;
  double h,x,sum=0.0,pi;

  h=1.0/(double)n;
  for(i=1;i<n;i++) {
    x=h*((double)i-(double)0.5);
    sum+=4.0/(1.0+x*x);
  }
  pi=h*sum;
  printf("n %ld Pi %lf\n",n,pi);
}
