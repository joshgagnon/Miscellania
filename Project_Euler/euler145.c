/*
sucks but  works. lol.

 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

#define LEN 10

int pos =9;
char *results;

int increment(char *num){
  int i;
  num[LEN-1]++;
  for(i=LEN-1;i>0;i--){
    if(num[i]>9){
      num[i]=0;
      num[i-1]++;
      if(i-1<pos) pos = i-1;
      }
  }
}



void print(char*num,int ord){
  int i;
  //printf("\n");
  for(i=ord;i<LEN;i++){
    printf("%d",num[i]);
  }
  printf("\n");
}


int allOdd(char*num,int ord){
  int i;
  for(i=LEN-1;i>=ord;i--){
    if(num[i]%2==0) return 0;
  }

  return 1;
}

void add(char*num,char*rev,int ord){
  int e=1,n=0,r=0;
  int i;
  for(i=LEN-1;i>=ord;i--){ 
    n+=num[i]*e;
    r+=rev[i]*e;
    e*=10;
  }
  results[n]=1; results[r]=1;
}


int reversible(char*num,int ord){
  int i,j,carry;
  char rev[LEN]={0,0,0,0,0,0,0,0,0,0};
  char sum[LEN]={0,0,0,0,0,0,0,0,0,0};
  if(num[ord]==0 || num[LEN-1]==0) return 0;
  for(i=ord,j=LEN-1;i<LEN;i++,j--){
    rev[i]=num[j];
    sum[i]=num[i]+num[j];      
  }
  for(i=LEN-1;i>=ord;i--){
    if(sum[i]>9){
      sum[i-1]+=sum[i] / 10;
      sum[i]=sum[i] % 10;
    }
  }
  
  /*printf("here: ");
  print(num,ord);
  print(sum,ord);*/
  if(sum[ord-1]>0){
    if( allOdd(sum,ord-1) ){
      ///print(sum,ord-1);
      ///printf("prev was success\n");
      //print(rev,ord);
      add(num,rev,ord);
      return 1;
    }
  }
  else{
    if(allOdd(sum,ord) ){
      //print(sum,ord);
      //printf("prev was success\n");
      add(num,rev,ord);
      //print(rev,ord);
      return 1;
    }
  }
  return 0;
}



int main(void){
  char num[LEN] ={0,0,0,0,0,0,0,0,0,1};
  int i = 0;
  int length = 100000000,count=0;
  results = malloc(length);
 while(++i<length){
   if(!results[i] && reversible(num,pos)) {count+=2;}
    //print(num,1);
		  //printf("%d\n",pos);
    increment(num);    
  }
 printf("%d\n",count);
}
