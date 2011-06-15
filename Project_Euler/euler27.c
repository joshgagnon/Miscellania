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

int countPrimes(char* primes, int a, int b){
	int hits = 0, value, n=0;
	do{
		value = n * n + (a*n) + b;
		if(primes[value] == 1) {	
			hits++;
		}
		else return hits;
		
	}while(++n);
	return -1;
}


int main(int argc, char**argv){
	char* primes = sieve(20000000);
	int a,b,hits,max=0;
	int range = 10000;
	for(a=-range+1;a<range;a++){
		for(b=-range+1;b<range;b++){
			hits = countPrimes(primes,a,b);
			if(hits>max){
				max = hits;
				printf("%d : a %d b %d\n",hits,a,b);
			}
		}
	}
	//printf("%d\n",countPrimes(primes,1,41));

return 0;
}