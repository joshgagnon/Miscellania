#include <stdlib.h>
#include <stdio.h>

/*
struct fract_t{
  int num;
  int dem;
};



typedef struct fract_t* fract;

int get_series(int i){
  int sum=0;
  do{ sum+=i--;}while(i>0);
  return sum;
}



int main(int argc, char **argv){
  int len = get_series(atoi(argv[1])-1);
  fract* fractions = malloc(len*sizeof*fractions);
  int i,x;
  printf("%d %d\n",len,atoi(argv[1])-1);
  fractions[0] = malloc(len*sizeof(*fractions[0])); 

  for(i=0;i<len;i++){
    fractions[i]=fractions[0] + sizeof(*fractions[0]) * i;

  }

  return 0;
}
*/

struct fract_t{
  int num;
  int dem;
};



typedef struct fract_t* fract;


int lessthan(fract goal, fract curr){
  return (double)goal->num/goal->dem > (double)curr->num/curr->dem;
}

int between(fract goal, fract curr, fract best){
  double a = (double)goal->num/goal->dem;
  double b = (double)curr->num/curr->dem;
  double c = (double)best->num/best->dem;
  return a > b && b > c;
}


int main(void){
  fract goal = malloc(sizeof(*goal));
  fract best = malloc(sizeof(*best));
  fract curr = malloc(sizeof(*curr));
  int i,j;
  goal->num=3; goal->dem=7;
  best->num=1; best->dem=5;
  for(i=2,j=2;i<1000000;i++){
    for(curr->num=j,curr->dem=i;lessthan(goal,curr);j++){
      curr->num=j;
      // printf("doing %d/%d\n",j,i);
      if(between(goal,curr,best)){
	best->num=curr->num; best->dem=curr->dem;
	}
     }	 
    }
printf("%d/%d\n",best->num,best->dem);
      
}
