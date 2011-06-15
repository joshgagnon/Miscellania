#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ALPHA 29
#define GROW 100000
#define ISALPHA(x) (((x)>='a' && (x)<='z') || (x)=='.' || (x)==',' || (x)==' ')
#define TRANS(x) (x) - (((x)>='a' && (x)<='z') ? 97 : ((x)=='.') ? 20 : ((x)==',') ? 17 : 4)
#define RESTORE(x) (x) + (((x)>=0 && (x)<=25) ? 97 : ((x)==26) ? 20 : ((x)==27) ? 17 : 4)


int main(int argc, char **argv){
	int count = 0,chr,i,j,k,m,tally,outputLength = 100000000;
	int size = GROW; double random,hmm[ALPHA][ALPHA][ALPHA][ALPHA];;
	char* text = malloc(GROW*sizeof(char)), *output = malloc(3*sizeof(char));
	while((chr=getchar())!=EOF){
		if(ISALPHA(chr)){
			text[count++] = TRANS(chr);
			if(count==size){
				size+=GROW;
				text = realloc(text,size);
			}
		}
	}
	for(i =0;i < ALPHA; i++)
		for(j =0;j < ALPHA; j++)
			for(k =0;k < ALPHA; k++)
				for(m =0;m < ALPHA; m++) 
					hmm[i][j][k][m]=0;
								
	for(i=3;i<count;i++) hmm[text[i-3]][text[i-2]][text[i-1]][text[i]] += 1;
	
	for(i =0;i < ALPHA; i++){
		for(j =0;j < ALPHA; j++){
			for(k =0;k < ALPHA; k++){
				tally = 0;
				for(m =0;m < ALPHA; m++) tally+=hmm[i][j][k][m];
				if(tally > 0){
				hmm[i][j][k][0] /= tally;
				for(m =1;m < ALPHA; m++) hmm[i][j][k][m] = hmm[i][j][k][m]/tally + hmm[i][j][k][m-1];
				}
			}
		}
	}
	output[0]=text[0]; output[1]=text[1]; output[2]=text[2];
	k = 1000; m = 1;
	printf("<DOC>\n<DOCNO> ABC%d-%d</DOCNO>\n",k++,i++);
	putchar(RESTORE(output[0])); putchar(RESTORE(output[1])); putchar(RESTORE(output[2]));	
	for(i=0;i<outputLength;i++){
		random = drand48();
		j = 0;
		while(random > hmm[output[0]][output[1]][output[2]][j]) j++;
		putchar(RESTORE(j));
		memmove(output,output+1,3);
		output[2]=j;
		if(RESTORE(j)=='.' && drand48() < 0.1) 
			printf("</DOC>\n<DOC>\n<DOCNO> ABC%d-%d</DOCNO>\n",k++,i++);
	}
	printf("</DOC>\n");
}