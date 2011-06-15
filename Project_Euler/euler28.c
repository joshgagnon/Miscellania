#include <stdio.h>
#include <stdlib.h>

#define DIM 1001



int main(void){
  int spiral[DIM][DIM];
  int x=DIM/2,y=DIM/2,i=1,lim=0,side=1,dir=0,sum=0;
 for(y=0;y<DIM;y++){
  for(x=0;x<DIM;x++){
   
      spiral[x][y]=0;
    }

  }
  x=DIM/2-1,y=DIM/2;
 
  while(i <= DIM*DIM){
    switch(dir){
    case(0):
      x++;
      break;
    case(1):
      y++;
      break;
    case(2):
      x--;
      break;
    case(3):
      y--;
      break;
    }
    side--;
    if(side == 0){
     dir = (dir+1) % 4;
     side = lim;
     if(dir%2!=0){
       side = lim = lim+1;
     }
    }
    spiral[x][y]=i++;

  }

  /*or(y=0;y<DIM;y++){
    for(x=0;x<DIM;x++){
      printf("%5d ",spiral[DIM-y-1][x]);
    }
    printf("\n");
    }*/
  for(y=0;y<DIM;y++){
    sum+=spiral[y][y];
    sum+=spiral[DIM-y-1][y];
  }
  printf("%d\n",sum-1);

  return 0;
}
