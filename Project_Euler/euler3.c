#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long* sieve(long max){
  long i,j,count,*primes;
  char *erato =  malloc(max*sizeof(char));  
  double sqr = sqrt(max); 

  erato[0]=0;
  erato[1]=0;

  for(i=2;i<max;i++) erato[i]=1;
  for(j=2;j<sqrt(max);j++) for(i=j*j; i<=max+1;i=i+j) erato[i]=0;
  for(i=0;i<max;i++) if(erato[i]==1) count++;
  printf("Primes: %ld\n", count);
  primes = malloc(count*sizeof *primes);
  i=0;
  for(j=0;j<max;j++) if(erato[j] == 1) primes[i++]=j;
  free(erato);
  return primes;
}

void factorise(long num, long* primes){
	long i=0;
	double max = sqrt(num);
	while(i<62113){
			
		if(num%primes[i]==0){
			num/=primes[i];
			printf("%ld\n",primes[i]);
		}else i++;	
	}
}

int main(int argc, char**argv){
	long* primes = sieve(sqrt(600851475144));
	factorise(600851475143, primes);
return 0;
}