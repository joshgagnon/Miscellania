#include <stdio.h>
#include <stdlib.h>

#define SIZE 80

void printSquare(int tri[][SIZE],int depth){
  int i,j;
  for(i=0;i<depth;i++){
    for(j=0;j<depth;j++){
      printf("%d ",tri[i][j]);
     } 
    printf("\n");
  }
}

void augment(int tri[][SIZE] ,int depth){
  int i,j;
  for(i=0;i<depth;i++){
    for(j=0;j<depth;j++){
      if(i==0 && j ==0);
      else if(i==0) tri[i][j]+=tri[i][j-1];
      else if(j==0) tri[i][j]+=tri[i-1][j];
      else{
	tri[i][j] += (tri[i][j-1] > tri[i-1][j]) ? tri[i-1][j] : tri[i][j-1];
      }
    }
  }
}

void printLargest(int **tri,int depth){
  int i,max = 0;
  for(i=0;i<=depth-1;i++){

    if(max < tri[depth-1][i]) max = tri[depth-1][i];
  }
  printf("result: %d\n",max);

}

int main(void){
  int depth,row=0,col=0,len=1;
  int squ[SIZE][SIZE];
  while(scanf("%d,",&squ[row][col++])==1){
    if(col==SIZE){
      col = 0;
      row++;
    }
  }
  //printSquare(squ,SIZE);
  augment(squ,SIZE);
  printf("\n Result: %d\n",squ[SIZE-1][SIZE-1]);
  //  augment(tri,row);
  //printTri(tri,row);
  //  printLargest(tri,row);
}
