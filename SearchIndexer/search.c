#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "types.h"

#define STARTSIZE 1024
#define MAXWORD 100
#define BLOCK 1024
#define INDEXWIDTH 1000

struct IndexRec indexPage;

unsigned char *memstore;

void printEntry(Entry e, int index){
  int i;
  printf("here index: %d\n",index);
  printf("%s %d \n",e[index].key,e[index].size);
  for(i=0;i<e[index].size;i++){
    printf("%d:%d, ",e[index].doc[i],e[index].hits[i]);
  }
  printf("\n");
}

int findInIndex(char *key){
  int high = indexPage.size, low = 0,mid;
    while(high - low > 1){
    mid=((unsigned int)low+(unsigned int)high)>>1;
    if(strcmp(indexPage.keys[mid],key)>0) high = mid;
    else low = mid;
  }
    return low;
}

int findInEntries(Entry e,char *key, int len){
  int high = len, low = -1,mid;
    while(high - low > 1){
    mid=((unsigned int)low+(unsigned int)high)>>1;
    if(strcmp(e[mid].key,key)>0) high = mid;
    else low = mid;
  }
  if(low == -1 || strcmp(e[low].key,key)!=0) return -1;
    return low;
}

unsigned char* readIndex(FILE *fp){
  unsigned char* store,*curr;
  size_t size;
  int err,i;
  struct IndexRec *indy;
  err = fread(&size,sizeof size, 1, fp);
  store = malloc(size);
  err = fread(store,1,size,fp);
  indy = (struct IndexRec*)store;
  curr = store + sizeof (struct IndexRec); 
  indy->start=(unsigned long*)curr;
  curr+=sizeof *indy->start*indy->size;
  indy->len=(unsigned char*)curr;
  curr+=sizeof *indy->len*indy->size;
  indy->keys=(unsigned char**)curr;
  curr+=sizeof *indy->keys*indy->size;
  for(i=0;i<indy->size;i++){
    indy->keys[i]=(char*)curr;
    curr+=indy->len[i]*sizeof *indy->keys[i];
  }
  indexPage = *indy;

  return store;
}

Entry readInEntries(FILE *fp,unsigned long from,unsigned long to,unsigned int *max){
  size_t totalSize = (size_t)(to-from);
  unsigned char *curr,*store = memstore;//malloc(totalSize);
  int err,i;
  Entry entries;
  fseek(fp,from,SEEK_SET);
  err = fread(store, 1, totalSize,fp);
  entries = (Entry)store; 
  curr = store + INDEXWIDTH * sizeof *entries;
  for(i=0;i<INDEXWIDTH;i++){
  	if(entries[i].len == 0) break; 
    entries[i].key = curr;	   
    curr += entries[i].len*sizeof(*entries[i].key) ;
    entries[i].doc = (unsigned int*)curr;
    curr += entries[i].size*sizeof(*entries[i].doc);  
    entries[i].hits = curr;
    curr += entries[i].size*sizeof(*entries[i].hits);   
  }
  *max = i;
  return entries;
}
 

int main(int argc, char **argv){
  FILE *fp;
  struct timeval start,stop;
  char buf[MAXWORD],query[]="h";
  unsigned int i=0,page,max;
  Entry entries;
  unsigned char* store;
  size_t filesize;
  memstore = malloc(1000000);
  fp = fopen("index.dat","r");
  if(!fp){
  	fprintf(stderr,"index.dat not found, exiting\n");
  	exit(1);
  }  
  store = readIndex(fp);
  fclose(fp);
  printf("Ready for queries:\n");
  fp = fopen("serial.dat","r");
  if(!fp){
  	fprintf(stderr,"serial.dat not found, exiting\n");
  	exit(1);
  }  
  fseek(fp,0L,SEEK_END);
  filesize = ftell(fp);
  fseek(fp,0L,SEEK_SET);
  
  while(scanf("%s",query)==1){
    gettimeofday(&start,NULL);
    page = findInIndex(query);
  
    if(page < indexPage.size-1 )entries = readInEntries(fp,indexPage.start[page],indexPage.start[page+1]-1,&max);
    else entries = readInEntries(fp,indexPage.start[page],filesize,&max);
    printf("%s is on index page %d after entry %s\n",query, page, indexPage.keys[page]);
    i=findInEntries(entries,query,max);
    gettimeofday(&stop,NULL);
    if(i==-1) printf("%s not found.\n",query);
    else {printf("%s found %d times in %.6f seconds.\n",query,entries[i].size,
		((double)stop.tv_sec+(double)(stop.tv_usec/1000000.0))-
		((double)start.tv_sec+(double)(start.tv_usec/1000000.0)));
		//printEntry(entries,i);
		}
    //free(entries);
  }
  fclose(fp);
  free(store);
  
 return 0;
}
