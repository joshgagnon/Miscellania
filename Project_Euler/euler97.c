#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

int main(){
	unsigned long long i = 1;
	unsigned int j;
	
	struct timeval tv,ev;
	gettimeofday(&tv, 0);
	
	for(j=0;j<7830457-30;j+=30){
		i<<=30;
		i%=10000000000;
	}
	i<<=7830457-j;
	i%=10000000000;
	
	i *= 28433;
	i%=10000000000;
	i++;
	gettimeofday(&ev, 0);
	printf("%llu in %dus\n",i,ev.tv_usec - tv.tv_usec);

}