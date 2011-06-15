#include <stdlib.h>
#include <stdio.h>

int sides = 15;

void go(int x,int y,int *count){
  //if(x == sides && y == sides) return sides;
  
  if(y<sides) (*count)++,go(x,y+1,count);
  if(x<sides)(*count)++, go(x+1,y,count);
  

}

/*

 ((sides+1)*(sides+1)*2 )/ len - 1;


 */


int main(void){
  int count=0;
  go(0,0,&count);
  printf("%d\n",count);
  //printf("%d\n",((sides+1)*(sides+1)));
}
