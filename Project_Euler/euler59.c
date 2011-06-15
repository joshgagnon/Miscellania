#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


#define SIZE 5000

void print(char *curr, int len){
  int i,sum=0;
for(i=0;i<len;i++){
  printf("%c",curr[i]);
  sum+=curr[i];
 }
 printf("\nsum=%d\n",sum);
}

void increment(char *num){
  int i;
  num[2]++;
    for(i=2;i>0;i--){
    if(num[i]>122){
      num[i]=97;
      num[i-1]++;
    }
  }
}

int main(void){
    char file[SIZE],curr[SIZE],xor[]={97,97,97}; 
    int i=0,j,len,hits[]={0,0};
  while(scanf("%d, ",(int*)&file[i++])==1);
  len = i-1;

  for(j=0;j<26*26*26;j++){
  for(i=0;i<len;i+=3){
    curr[i]=file[i]^xor[0];
    curr[i+1]=file[i+1]^xor[1];
    curr[i+2]=file[i+2]^xor[2];
  }
   hits[0]=0;hits[1]=0;
  for(i=0;i<len-4;i++){
       if(tolower(curr[i])=='a' && tolower(curr[i+1])=='n' && tolower(curr[i+2])=='d'){
       	hits[0]++;
       }
        if(tolower(curr[i])=='o' && tolower(curr[i+1])=='f'){
       	hits[1]++;
       }
  }
  if(hits[0]>1	 && hits[1]>1){
    printf("\n\ntrying %c %c %c\n\n",(char)xor[0],(char)xor[1],(char)xor[2]);
    print(curr,len);
    
  }
  increment(xor);
  }  
  
  
  
}
