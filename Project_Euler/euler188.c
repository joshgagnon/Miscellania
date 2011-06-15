#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

typedef unsigned long long _ull;


_ull modpower(_ull base, _ull exp, _ull mod){
	_ull res = 1;
	while(exp>0){
		if((exp & 1) == 1){
			res = (res*base) % mod;
		}
		exp>>=1;
		base = (base*base) % mod;
	}	
	return res;
}



int main(void){
	_ull mod = 100000000,base = 1777,exp = 1855,old;
	_ull run = base;
	struct timeval tv,ev;
	gettimeofday(&tv, 0);
	int i=1;
	while(i < exp){
		old = run;
		run = modpower(base,run,mod);
		if(run == old) break;
		i++;
	}
	gettimeofday(&ev, 0);
	printf("in %dus\n",ev.tv_usec - tv.tv_usec);
	
	printf("%llu\n",run);
}