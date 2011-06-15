/*lol, while the solution is quick, its stupid

should have done:

	for a (3..1000){
		sum += (a % 2 == 0) ? a*(a-2) : a*(a-1);
	done
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

typedef unsigned long long int uint;



uint modpower(uint base, uint exp, uint mod){
	uint res = 1;
	while(exp>0){
		if((exp & 1) == 1){
			res = (res*base) % mod;
		}
		exp>>=1;
		base = (base*base) % mod;
	}	
	return res;
}


uint getR(int a, int n){
	return (modpower(a-1,n,a*a) + modpower(a+1,n,a*a)) % (a*a);
}

int main(void){
	uint a,b,max,t,sum=0;;
	for(a=3;a<=1000;a++){
		max =0;
		for(b=1;b<=a*2;b++){
			t = getR(a,b);
			if(max < t){
				max = t;
			}			
		}
		sum+=max;
	}
	printf("%llu\n",sum);
}

