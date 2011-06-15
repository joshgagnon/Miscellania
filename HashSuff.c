#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BLOCKSIZE 10000000 	/*For reading in*/
#define START_SEQS 1000    	/*Default number of sequences*/
#define PAT_MAX 32			/*Maximum pattern length*/
#define QSORTVALUE 0		/*quicksort*/

unsigned long codes[117];
const char letters[] = "ACGU";

struct suffix_rec{
  unsigned int length, sequences, *positions;
  unsigned long *hashes;
};

typedef struct suffix_rec *suffices;

void init(){
  codes[65]=0; codes[67]=1; codes[71]=2;  codes[85]=3;
  codes[97]=0; codes[99]=1; codes[103]=2; codes[117]=3;
}


unsigned long make_mask(const int k){
  int i;	unsigned long result = 0;
  for(i=0;i<k*2;i++) result |= 1 << i;
  return result;
}

unsigned long make_mask_left(const int k, const int kmer){
  int i;	unsigned long result = 0;
  for(i=(kmer-k)*2;i<kmer*2;i++) result |= 1 << i;
  return result;
}

unsigned long make_hash(const char *s, const int k){
  int i;	unsigned long result = 0;
  for(i=0;i<k;i++) result |= codes[s[i]] << (k-i-1)*2; 	 
  return result;
}

void printHash_asString(unsigned long num, int k){/*k side effect*/
  static const unsigned long bp_mask = 3;	
  while(k>0) putc(letters[(num>>((k---1)*2)) & bp_mask],stdout);
}

char* hash_to_string(unsigned long num, char* string, const int k){
  static const unsigned long bp_mask = 3;	int i=k;
  while(i>0) string[k-i--]=letters[(num>>((i-1)*2)) & bp_mask];
  string[k]='\0';
  return string;
}
unsigned int combinePosSeq(unsigned int a, unsigned int b){
  return b | (a<<10);
}

unsigned int getPos(unsigned int a){
  return a>>10;
}

unsigned int getSeq(unsigned int a){
  return a & make_mask(5);
}

void sortArrayM(unsigned long *a, unsigned long *aw, unsigned int *b, unsigned int *bw,  int n){
  int i,j,k,m;
  if(n<2) return; 
  sortArrayM(a,aw,b,bw,n/2);
  sortArrayM(a+n/2,aw+n/2,b+n/2,bw+n/2,n-(n/2));
  m=n/2; i=0; k=0; j=m;  
  while(i<m && j<n){
    if(a[i]<=a[j]){	aw[k]=a[i]; bw[k++]=b[i++]; }
    else { 			aw[k]=a[j]; bw[k++]=b[j++]; }  		
  }
  while(i<m){ aw[k]=a[i];  	bw[k++]=b[i++]; }
  while(j<n){ aw[k]=a[j];	bw[k++]=b[j++]; }
  for(i=0;i<n;i++){ a[i]=aw[i]; b[i]=bw[i];	}
}

void sortArrayQ(unsigned long *arr, unsigned int *pos, int beg, int end){
  unsigned long tl; int ti;
  if (end > beg + 1){
    unsigned long piv = arr[beg], l = beg + 1, r = end;
    while (l < r){
      if (arr[l] <= piv) l++;
      else {
	r--;
	tl = arr[l]; arr[l] = arr[r]; arr[r] = tl;
	ti = pos[l]; pos[l] = pos[r]; pos[r] = ti;
      }
    }
    l--;
    tl = arr[l]; arr[l] = arr[beg]; arr[beg] = tl;
    ti = pos[l]; pos[l] = pos[beg]; pos[beg] = ti;	
    sortArrayQ(arr, pos, beg, l);
    sortArrayQ(arr, pos, r, end);
  }
}


void sortAll(suffices suffix, unsigned int kmer){
  int i,max=0; unsigned long *aw; unsigned int *bw;
  if(QSORTVALUE) sortArrayQ(suffix->hashes,suffix->positions,0,suffix->length);
  else{
    aw = malloc(suffix->length*sizeof *aw); 
    bw = malloc(suffix->length*sizeof *bw);
    sortArrayM(suffix->hashes,aw,suffix->positions,bw,suffix->length);
    free(aw); free(bw);// free(cw);
  }
}

suffices read_input(FILE *fp,unsigned int kmer){
  char abuf[PAT_MAX+1],cbuf;
  unsigned int size = BLOCKSIZE,currPos=0,seqCount=0;
  unsigned long kmer_mask = make_mask(kmer);
  suffices suffix = malloc(sizeof *suffix);
  suffix->hashes = malloc(size*sizeof *suffix->hashes);
  suffix->positions = malloc(size*sizeof *suffix->positions);
  suffix->length = 0;
  while(1){
    if(!fgets(abuf,kmer+1,fp)) return suffix;
    if(strlen(abuf)<kmer) continue;
    currPos = 0;
    suffix->hashes[suffix->length] = make_hash(abuf,kmer);
    suffix->positions[suffix->length] = combinePosSeq(currPos++,seqCount);
    suffix->length++;
    while((cbuf = fgetc(fp))!='\n'){
      if(cbuf==EOF) return suffix;
      if(size == suffix->length){
	size += BLOCKSIZE;
	suffix->hashes = realloc(suffix->hashes,size*sizeof *suffix->hashes);
	suffix->positions = realloc(suffix->positions,size*sizeof *suffix->positions);				
      }
      suffix->hashes[suffix->length] = ((suffix->hashes[suffix->length-1] << 2) & kmer_mask) | codes[cbuf];
      suffix->positions[suffix->length] = combinePosSeq(currPos++,seqCount);
      suffix->length++;
    }
    seqCount++;
    suffix->sequences=seqCount;
  }
}



void findInteresting(suffices suffix, unsigned int kmer){
  int i=0,k,count; unsigned long curr, left_mask;
  unsigned char* table = malloc(suffix->sequences);
  char buf[PAT_MAX];
  for(k=3;k<kmer;k++){
  i=0;
  left_mask = make_mask_left(k,kmer);
  curr = suffix->hashes[0] >> ((kmer-k)*2);
  while(i<suffix->length){
    count=0;
    memset(table,0,sizeof (unsigned char));
    while(i<suffix->length && suffix->hashes[i]>> ((kmer-k)*2) ==curr) count++,i++;
    if(i==suffix->length)continue;
    //if(count > (double)suffix->sequences*0.1)
    printf("%s %d\n",hash_to_string(curr>> ((kmer-k)*2),buf,k),count);
    
    curr = suffix->hashes[i]>> ((kmer-k)*2);			
    }
}
 
}


int main(int argc, char **argv){
  FILE *fp;
  suffices suffix;
  char buf[PAT_MAX];
  int n,i, kmer=PAT_MAX,numSeq=0;
  unsigned long kmer_mask = make_mask(kmer);
	
  if(argc < 2 || !(fp = fopen(argv[1],"r"))){
    fprintf(stderr,"Please Specify a sequence file\n");
    return 1;
  }
  if(argc == 3 && atol(argv[2]) > 7 && atol(argv[2]) < PAT_MAX) kmer = atol(argv[2]);		
  init();	
  suffix = read_input(fp,kmer);
  sortAll(suffix,kmer);
  findInteresting(suffix,kmer);
  printf("suffix length = %d\n",suffix->length);
  for(i=0;i<suffix->length;i++){
    //if(i%1000000==0) 
    //printf("%s %d %d\n",hash_to_string(suffix->hashes[i],buf,kmer),getPos(suffix->positions[i]),getSeq(suffix->positions[i]));
    }
  printf("%s pew\n",hash_to_string(make_mask_left(3,8),buf,8));

  free(suffix->positions);
  free(suffix->hashes);
  free(suffix);
}




