
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long sieve(long max,int limit){
  long i,j,count;
  char *erato =  malloc(max*sizeof(char));  
  double sqr = sqrt(max); 

  erato[0]=0;
  erato[1]=0;

  for(i=2;i<max;i++) erato[i]=1;
  for(j=2;j<sqrt(max);j++) for(i=j*j; i<=max+1;i=i+j) erato[i]=0;
  for(i=0;i<max;i++) if(erato[i]==1) { count++; if(count==limit) return i;}
  return 0;
}

int main(int argc, char**argv){
	printf("%ld\n",sieve(100000000,10001));
return 0;
}
