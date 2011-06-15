/*
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define LEN 8

int facts[10]={1,1,2,6,24,120,720,5040,40320,362880};
int num[LEN];

void increment(){
  int i;
  num[LEN-1]++;
    for(i=LEN-1;i>0;i--){
    if(num[i]>9){
      num[i]=0;
      num[i-1]++;
    }
  }
}

void print(){ 
  int i;
  for(i=0;i<LEN;i++){
    printf("%d",num[i]);

  }
  printf("\n");
}

unsigned int get_sum(int sig){
  int i,sum=0;
  //print();
  for(i=0;i<sig;i++){
  	//printf("%d ",num[LEN-1-i]);
    sum+=facts[num[LEN-1-i]];
  }
  //printf(": %d \n",sum);
  return sum;
}

int main(void){
  unsigned int i=0,max = 999999,order=2,res=0;;
  for(i=0;i<LEN;i++){
  	num[i]=0;
  }
  i=10;
  
  num[LEN-2]=1;
  while(i<max){
    if(get_sum(order)==i){
    	res+=i;
      printf("%d: ",i);print();
    }
  increment();
 i++;
 if(i==100) order=3;
 if(i==1000) order=4;
 if(i==10000) order =5;
 if(i==100000) order=6;
 if(i==1000000) order=7;
  }
printf("answer is %d\n",res);
}
