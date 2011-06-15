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


char* rotate(char *word){
	char c = word[0];
	word[strlen(word)] = c;
	return ++word;
}


int main(int argc, char**argv){
	char* primes = sieve(1000000);
	char *word = malloc(100);
	int x,y,num;
	int range = 10000;
	for(x=0;x<1000000;x++){
		if(primes[x]){
			sprintf(word,"%d",x);
			//printf("%s\n",word);
			for(y=0;y<strlen(word);y++){
				word = rotate(word);
				sscanf(word,"%d",&num);
				if(!primes[num]){
					break;
				}
			}
			if(y==strlen(word)){
				printf("%d\n",x);
			}
		}
	}


return 0;
}