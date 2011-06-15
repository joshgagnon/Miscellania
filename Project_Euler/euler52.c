#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define LEN 10



void increment(int *num){
  int i;
  num[LEN-1]++;
    for(i=LEN-1;i>0;i--){
    if(num[i]>9){
      num[i]=0;
      num[i-1]++;
    }
  }
}

void multiply(int *num, int *next, int multi){
  int i,carry=0;
  
    for(i=LEN-1;i>0;i--){
    next[i]=num[i]*multi+carry;
    carry=0;
    if(next[i]>9){
      carry = next[i]/10;
      next[i]=next[i]%10;
  
    }
  }
}

void print(int *num){ 
  int i;
  for(i=0;i<LEN;i++){
    printf("%d ",num[i]);

  }
  printf("\n");
}

int comp(const void *a, const void *b){
  return *(char*)a < *(char*)b;
}
int samedigits(int *a, int *b){
  int sorta[LEN], sortb[LEN];
  int i;
  memcpy(sorta,a,sizeof(int)*LEN);
  memcpy(sortb,b,sizeof(int)*LEN);
  qsort(sorta,LEN,sizeof(int),comp);
  qsort(sortb,LEN,sizeof(int),comp);
  for(i=0;i<LEN;i++){
    if(sorta[i]!=sortb[i]) return 0;

  }
  return 1;

}

int main(void){
  int count=0,i;
    int num[LEN]={0,0,0,0,0,0,0,0,0,0},next[LEN]={0,0,0,0,0,0,0,0,0,0};
    while(count<999999){
      increment(num);
      for(i=2;i<7;i++){
	multiply(num,next,i);
      if(!samedigits(num,next)){
	break;
      }
    }
      if(i==7){
	printf("----------\n");
	print(num);
	print(next);
	exit(1);
      }
      
}
    
}
