#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int multi_a[] = {0,0,0,0};
int multi_b[] = {0,0,0,0};
int multi_prod[] = {0,0,0,0,0,0,0};

char nums[]={0,0,0,0,0,0,0,0,0};
int len = 9;

void reset(){
  int i;
  for(i=0;i<4;i++){
    multi_a[i]=0;
    multi_b[i]=0;
    multi_prod[i]=0;
  }
  multi_prod[i++]=0;
  multi_prod[i++]=0;
  multi_prod[i]=0;

}

int is_pandigital(){
  int numbers[] = {0,0,0,0,0,0,0,0,0};
  int i;
  for(i=0;i<9;i++){
    if(numbers[i]) return 0;
    numbers[i]=1;

  }
  return 1;
}

int is_pan(int x, int y, int z){
  static char str[100];
  char numbers[] = {0,0,0,0,0,0,0,0,0}; 
  int i,slen = sprintf(str,"%d%d%d",x,y,z);
  
  if (slen != len) return 0;
  for(i=0;i<len;i++){
    if(str[i]=='\0' || numbers[str[i]-49]) return 0;
    numbers[str[i]-49]=1;
  }
  return 1;
}



int main(void){
  int i,j;
  int sum=0;
  unsigned char *result = calloc(100000,1);
  for(i=1;i<999;i++){
    for(j=i;j*i<9999;j++){
      if(is_pan(i,j,i*j))result[i*j]=1;
    }
  }
  for(i=0;i<100000;i++){
    if(result[i]) sum+=i;

  }
  printf("%d\n",sum);
  return 0;
}
