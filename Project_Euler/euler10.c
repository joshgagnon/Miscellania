#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char* sieve(long max){
  long i,j,count=0;
  char *erato =  malloc(max*sizeof(char));  
  double sqr = sqrt(max); 

  erato[0]=0;
  erato[1]=0;

  for(i=2;i<max;i++) erato[i]=1;
  for(j=2;j<sqrt(max);j++) for(i=j*j; i<=max+1;i=i+j) erato[i]=0;
  for(i=0;i<max;i++) if(erato[i]==1) count++;
  printf("Primes: %ld\n", count);
  return erato;
}


int main(int argc, char**argv){
	char* primes = sieve(2000000);
	long sum = 0;
	int i=2;
	while(i<2000000){
		if(primes[i]==1) sum+=i;
		i++;
	}
	printf("%ld\n",sum);
return 0;
}