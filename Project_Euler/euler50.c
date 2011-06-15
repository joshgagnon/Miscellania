#include <stdio.h>
#include <string.h>
#include <stdint.h>
 
 
 
#define MAXN  1000000  /* maximum value of N */
#define P1    15625    /* = ceil(MAXN/64) */
#define P2    500000   /* = ceil(MAXN/2) */
#define P3    500       /* = ceil(ceil(sqrt(MAXN))/2) */
#define P4    78498    /* = number of primes < 1000000*/

uint32_t sieve[P1];
uint32_t primes[P4];
 
#define GET(b) ((sieve[(b)>>5]>>((b)&31))&1)
 
void make()
{
    uint32_t i, j, k;
    memset(sieve, 0, sizeof(sieve));
    for (k = 1; k <= P3; k++)
        if (GET(k)==0) for(j=2*k+1,i=2*k*(k+1);i<P2;i+=j) sieve[i>>5]|=1<<(i&31);
}
 
int isprime(int p) { return p==2 || (p>2 && (p&1)==1 && (GET((p-1)>>1)==0)); }
 
int sum(int i, int j) { int sum=0; for (;i<=j;i++)sum+=primes[i]; return sum;} 

int print(int i, int j) { for (;i<=j;i++) printf("%d\n",primes[i]); } 

int contains(int i) {int j; for(j=0;j<P4;j++) if (primes[j]==i) return 1; return 0; }

int main()
{
  int i,j, n;
  int x=0,y,z;
    make();
    for (n = 0, i = 0; i <= MAXN; i++)
        if (isprime(i)) primes[n++]=i;
    for(i=0;i<P1;i++){
      for(j=i+1;n=sum(i,j),j<i+2000 && j<MAXN && n<MAXN;j++){
	if(isprime(n)){
	  if(j-i>x)x=j-i,y=i,z=j;
	}
      }
    }
    printf("%d %d %d %d\n",x+1, sum(y,z), primes[y],primes[z]);
    return 0;
}
