#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int perm[]={0,1,2,3,4,5,6,7,8,9};
unsigned int len = 10;
unsigned int **results;

unsigned int sum[100],sumlen=100,pos=1;


unsigned int fact(unsigned int i){
  if(i==1) return 1;
  return i * fact(i-1);
}

void print(){
  int i;
  for(i=0;i<len;i++){
    printf("%d",perm[i]);
  }
  printf("\n");
}

void printResult(unsigned int *x){
  int i;
  for(i=0;i<len;i++){
    printf("%d",x[i]);
  }
  printf("\n");
}

void print_sum(){
  int i;
  for(i = sumlen-pos+1;i<sumlen;i++){
    printf("%d",sum[i]);
  }
  printf("\n");


}

void calc_sum(unsigned int **result, unsigned int count){
  int i,j,carry=0;
  for(j=len-1;j>=0;j--){
    carry;
    for(i=0;i<count;i++){
      carry+=result[i][j];
    }
    sum[sumlen-pos++] = carry % 10;
    carry/= 10;
  }
  while(carry!=0){
    sum[sumlen-pos++] = carry % 10;
    carry/= 10;

  }
}


void swap(unsigned int *a, unsigned int *b){
  unsigned int t;
  t = *a;
  *a = *b;
  *b = t;

}

void reverse(unsigned int i){
  unsigned int j=len-1;
  while(j>i){
    swap(&perm[i],&perm[j]);
    j--;i++;
  }
  
}

int next_perm(){
  unsigned int i, k = -1, l = 0,t;
  for(i=0;i<len-1;i++) 
    if(perm[i] < perm[i+1]) 
      k = i;
  if(k==-1) return 0;
  l = k+1;
  for(i=k+1;i<len;i++)
    if(perm[k] < perm[i]) 
      l = i;
  
 
  swap(&perm[k],&perm[l]);
  reverse(k+1);
  return 1;
}


int unusual(){
  static const unsigned int series[] = {2,3,5,7,11,13,17};
  static const unsigned int tests = 7; 
  unsigned int num, i;
  for(i=1;i<=tests;i++){
    num = 100*perm[i]+10*perm[i+1]+perm[i+2];
    if(num % series[i-1] != 0) return 0;
  }
  return 1;
}


int main(void){
  unsigned int digit = 10, limit = fact(10);
  unsigned int j,sum=0,hits=0;
   
  while(next_perm()){
    if(unusual()){
      results = realloc(results,(hits+1)*sizeof *results);
      results[hits] = malloc(len*sizeof *results[0]);
      memcpy(results[hits],perm,len*sizeof perm[0]);
      //printf("%d : ",j);
      printResult(results[hits]);
      hits++;
    }
    //next_perm();
  }
  calc_sum(results,hits);
  print_sum();
  //printf("sum is %d\n",sum);
  
  return 0;
}
