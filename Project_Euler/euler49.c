#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int sort(const void *x, const void *y) {
  return (*(char*)x - *(char*)y);
}

int isPermutation(int a, int b){
	char adig[4] = {a/1000,(a%1000)/100,(a%100)/10,(a%10)}, bdig[4] = {b/1000,(b%1000)/100,(b%100)/10,(b%10)};
	qsort(adig, 4, sizeof(char),sort);
	qsort(bdig, 4, sizeof(char),sort);
	if(a != b && (*(int*)adig) == (*(int*)bdig)) return 1;
	return 0;
}
/*would be much faster to generate permutations and test*/
int main(int argc, char**argv){
	char* primes = sieve(10000);
	char *word = malloc(100);
	int x,y,z;
	int range = 10000;
	printf("%d\n",isPermutation(1234,4321));
	for(x=1000;x<10000;x++){
		if(primes[x]){
			for(y=x+1;y<10000;y++){
				if(primes[y]){
					for(z=y+1;z<10000;z++){
						if(primes[z] && isPermutation(x,y) && isPermutation(y,z) && y-x == z-y){
							printf("%d %d %d\n",x,y,z);
						
						}
					}
				
				}
			}
		}
	}


return 0;
}