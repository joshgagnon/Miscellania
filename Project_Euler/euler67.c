#include <stdio.h>
#include <stdlib.h>

void printTri(int **tri,int depth){
	int i,j;
	for(i=0;i<depth;i++){
		for(j=0;j<=i;j++){
		printf("%d ",tri[i][j]);
		}
		printf("\n");
		}
}

void augment(int **tri,int depth){
	int i,j;
	for(i=1;i<depth;i++){
		for(j=0;j<=i;j++){
			if(j==0) tri[i][j]+=tri[i-1][0];
			else if(j==i) tri[i][j]+=tri[i-1][i-1];
			else{
				tri[i][j] += (tri[i-1][j-1] > tri[i-1][j]) ? tri[i-1][j-1] : tri[i-1][j];
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
	int **tri,depth,row=0,col=0,len=1;
	tri = malloc(101*sizeof(*tri));
	tri[row]=malloc(sizeof(int));
	while(scanf("%d",&tri[row][col++])==1){
		if(col==len){
			col = 0;
			len++;
			row++;
			tri[row]=malloc(sizeof(int)*len);
		}
	}
	//printTri(tri,row);
	augment(tri,row);
	//printTri(tri,row);
	printLargest(tri,row);
}