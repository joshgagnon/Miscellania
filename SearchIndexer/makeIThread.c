#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "types.h"

#define HASHSIZE 60466176
#define TOTALMEMORY (1024*1024*1024+1024*1024*700)
#define MAXWORD 100
#define STARTSIZE 10
#define INDEXWIDTH 1000
#define NUMTHREADS 10


unsigned char* mem;
unsigned char* nextmem;
unsigned char* serialStore, *currStore;
unsigned char magicChar[]="#";
unsigned char magicNULL[]="\0";
unsigned char prefix[4];
unsigned int totalEntries = 0;

Entry entries;
struct Node** hash;
struct IndexRec indexPage;
struct fileBufferRec fileBuffer[NUMTHREADS];
pthread_mutex_t** mutexes;

/*****************************************************************/
/* Thread support                                                */
/*                                                               */
/*****************************************************************/
inline unsigned char* requestMemory(size_t i){
	static pthread_mutex_t memtex=PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock( &memtex );
	unsigned char* temp =nextmem;
	nextmem += i;
	pthread_mutex_unlock( &memtex );
	return temp;
}

inline void upCount(){
	static pthread_mutex_t counttex=PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock( &counttex );
	totalEntries++;
	pthread_mutex_unlock( &counttex );
}

inline void createNewMutex(int hashval){
	static pthread_mutex_t createtex=PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock( &createtex );
	mutexes[hashval] = (pthread_mutex_t*)requestMemory(sizeof(**mutexes));
	pthread_mutex_init(mutexes[hashval],NULL);
	pthread_mutex_unlock( &createtex );
	
}

/*****************************************************************/
/* Binary Tree.                                                  */
/*                                                               */
/*****************************************************************/

/* Updates node.
 * Hit counts and docnos are a linked list, with each linked list node being STARTSIZE long. */
inline void update(struct Node *this, unsigned int docno){
	unsigned char *temp;
  if(this->currdoc[0]==docno && this->currhit[0]<255) this->currhit[0]++;
  else if(this->currdoc[0]!=docno){
    if(this->remain>1){

      this->currdoc++;
      this->currdoc[0]=docno;
      this->currhit++;
      this->currhit[0]=1;
      this->remain--;
      this->count++;
    }
    else{   	
        this->currdoc++;
		this->currhit++;
		this->order++;	
		this->remain = STARTSIZE; 
		temp = requestMemory(this->remain*sizeof(unsigned int));
		this->currdoc[0]=(unsigned int)(temp - (unsigned char*)this->docs);
		this->currdoc=(unsigned int*)temp;
		this->currdoc[0]=docno;		
		this->currhit[0]=0;
		this->currhit = requestMemory(this->remain);
       this->currhit[0]=1;
       this->remain--;
       this->count++;
    }
   }

}

/* Insert word into tree.  Converted to iteration to help compiler.
 * 	Pattern is :
 *		- word is 3 letters long, resulting in single node tree, and tree exists
 *		- word has a match, add doc no
 *		- word has no match, create node
 *		- tree is empty, create new tree */
/*heaps to optimize*/
inline void insert(unsigned int pos, unsigned char* word,unsigned char len,unsigned int docno){
  struct Node *this = hash[pos],*parent; 
  int cmp;
  enum side_e {LEFT,RIGHT} side=LEFT;
  if(this!=NULL && hash[pos]->len==4 && len == 4){
    update(hash[pos],docno);
    return;
  }
  else if(this!=NULL){
    while(this!=NULL){
      cmp = strcmp((char*)this->key,(char*)word);//must replace
      if(cmp==0){
	update(this,docno);
	return;
      }
      parent = this;
      if(cmp<0) this=this->right, side=RIGHT;
      else  this=this->left , side=LEFT;
    }
    /*create new node*/
    this = (struct Node*)requestMemory(sizeof (struct Node));

    if(side==RIGHT) parent->right=this;
    else parent->left = this;
    this->key = word;
    this->currdoc = this->docs = (unsigned int*)requestMemory(STARTSIZE*sizeof(unsigned int));
    this->currhit = this->hits = requestMemory(STARTSIZE);  
    this->len=len;
    this->count=1;
    this->docs[0]=docno;
    this->hits[0]=1;
    this->order=0;
    this->remain=STARTSIZE-1;    
    this->left=this->right=NULL;  
    upCount();
    return;
  }
  
  else{ 
    hash[pos]=(struct Node*)requestMemory(sizeof (struct Node));
    this = hash[pos];
    this->key = word;
    this->currdoc = this->docs = (unsigned int*)requestMemory(STARTSIZE*sizeof(unsigned int));
    this->currhit = this->hits = requestMemory(STARTSIZE);   
    this->len=len;
    this->count=1;
    this->docs[0]=docno;
    this->hits[0]=1;
    this->order=0;
    this->remain=STARTSIZE-1;    
    this->left=this->right=NULL;
    upCount();
  }
}


void traverseDocs(struct Node* e){
	int i,pos; 
	unsigned char *currhit = e->hits;
 	unsigned int *currdoc = e->docs;
	printf("Doing %s with %d docs\n",e->key,e->count);
	for(i=0,pos=0;pos<e->count;i++){
		if(i%STARTSIZE==STARTSIZE-1){
			printf("fixing\n");
			currdoc = (unsigned int*)(currdoc[0] + (unsigned char*)e->docs);
			currhit = (unsigned char*)currdoc + STARTSIZE*sizeof(*currdoc);
		}	
		else{printf(" %d x %d,\n",currdoc[0],currhit[0]);
			pos++;
			currdoc++;
			currhit++;
		}
	}
}


/*****************************************************************/
/*  Serialization                                 				 */
/*                                                               */
/*****************************************************************/
void initIndex(unsigned int indexSize){
  indexPage.keys = malloc(indexSize*sizeof *indexPage.keys);
  indexPage.start = malloc(indexSize*sizeof *indexPage.start);
  indexPage.len = malloc(indexSize*sizeof *indexPage.len);  
  indexPage.size=indexSize;
}

void traverse(struct Node* e, int *count ){
  if(e!=NULL){
    traverse(e->left,count);
    (*count)++;
    traverse(e->right,count);
  }
}

void serializeTree(struct Node* e, FILE *fp, unsigned int *count, unsigned int *indexPos){
  int res,i; unsigned int offset;
  unsigned char *hitStore, *docStore;
  if(e){
    serializeTree(e->left,fp,count,indexPos);
    	
      if(*count%INDEXWIDTH == 0){
      
		indexPage.keys[*indexPos] = e->key;
		indexPage.len[*indexPos] = e->len;
		indexPage.start[*indexPos]=ftell(fp);
		
		(*indexPos)++;
      }         	
		entries[*count%INDEXWIDTH].len = e->len;
		entries[*count%INDEXWIDTH].size = e->count;
		memcpy(currStore,e->key,e->len*sizeof *e->key);
		currStore += e->len*sizeof *e->key;
		
		docStore = currStore;
		currStore += e->count * sizeof (*e->docs);
		hitStore = currStore;
		currStore += e->count * sizeof (*e->hits);
		i = 0;
		e->currdoc = e->docs;
		e->currhit = e->hits;
		
		while(e->count){

			if (e->count < STARTSIZE){
				memcpy(docStore,e->currdoc,e->count*sizeof *e->docs);
				docStore += e->count*sizeof *e->docs;	
				memcpy(hitStore,e->currhit,e->count*sizeof *e->hits);
				docStore += e->count*sizeof *e->hits;					
				break;
			}
			else{
				memcpy(docStore,e->currdoc,(STARTSIZE-1)*sizeof *e->docs);
				docStore += (STARTSIZE-1)*sizeof *e->docs;	
				memcpy(hitStore,e->currhit,(STARTSIZE-1)*sizeof *e->hits);		
				hitStore += (STARTSIZE-1)*sizeof *e->hits;			
				e->currdoc = (unsigned int*)((unsigned char*)e->docs + e->currdoc[STARTSIZE-1]);
				e->currhit = (unsigned char*)e->currdoc + STARTSIZE*sizeof(*e->currdoc);
				e->count -= STARTSIZE -1;
				
			}			
		}	
		
    	/*Flatten and copy*/
  		if(*count%INDEXWIDTH == INDEXWIDTH-1){
  			res=fwrite(entries,1,currStore - (unsigned char*)entries,fp); 			
  			currStore = serialStore;
  		}
  		if(*count == totalEntries-1){
  			entries[(*count+1)%INDEXWIDTH].len=0;
  			res=fwrite(entries,1,currStore - (unsigned char*)entries,fp); 			
  			currStore = serialStore; 		  		
  		}    
    (*count)++;
    serializeTree(e->right,fp,count,indexPos);
  }
}


void serializeIndex(FILE *fp){
  size_t totalSize=0;
  unsigned int i;
  unsigned char *store,*curr;
  totalSize+=sizeof indexPage;
  printf("Starting index\n");
  for(i=0;i<indexPage.size;i++){
    totalSize+=(indexPage.len[i]*sizeof*indexPage.keys[i]);
  }  
  totalSize+=indexPage.size*sizeof *indexPage.start;
  totalSize+=indexPage.size*sizeof *indexPage.len; 
  totalSize+=indexPage.size*sizeof *indexPage.keys; 
  store=requestMemory(totalSize);//malloc(totalSize);
  //nextmem += totalSize;
  fwrite(&totalSize, sizeof totalSize,1,fp);
  
  memcpy(store,&indexPage,sizeof indexPage);
  curr = store + sizeof indexPage;
  memcpy(curr,indexPage.start,indexPage.size * sizeof *indexPage.start);
  curr += indexPage.size * sizeof *indexPage.start;
  memcpy(curr,indexPage.len,indexPage.size * sizeof *indexPage.len);
  curr += indexPage.size * sizeof *indexPage.len;
  memcpy(curr,indexPage.keys,indexPage.size * sizeof *indexPage.keys);
  curr += indexPage.size * sizeof *indexPage.keys;
  for(i=0;i<indexPage.size;i++){
    memcpy(curr,indexPage.keys[i],indexPage.len[i]*sizeof*indexPage.keys[i]);
     
    curr+=indexPage.len[i]*sizeof*indexPage.keys[i];  
  }
  fwrite(store,1,totalSize,fp);
}

/* Traverse and flatten the tree, and build the index */
void serializeAll(){
  FILE *index = fopen("index.dat","w");
  FILE *db = fopen("serial.dat","w");
  unsigned int i = 0,count = 0, indexPos=0;
  int res;
  printf("Serializing\n");
  entries = (Entry)requestMemory(INDEXWIDTH * sizeof *entries);
  
  serialStore = currStore = nextmem;
  initIndex(totalEntries/INDEXWIDTH+1);
  while(i < HASHSIZE){
    if(hash[i]!= NULL) {  
      serializeTree(hash[i],db,&count,&indexPos); 
    }
    i++;
  }
  serializeIndex(index);
}


/*****************************************************************/
/* File Input                                                    */
/*                                                               */
/*****************************************************************/

inline unsigned int enumerateChar(char c){
  if(c>=97 && c<=122) return (unsigned int)c-87;
  return (unsigned int)c-48;

}

/* a hash function */
inline unsigned int enumerateWord(unsigned char* word){
  unsigned int total=0;
  total += 36*36*36*36*enumerateChar(word[0]);
  if(word[1]!='\0') total += 36*36*36*enumerateChar(word[1]); else return total;
  if(word[2]!='\0') total += 36*36*enumerateChar(word[2]); else return total;
  if(word[3]!='\0') total += 36*enumerateChar(word[3]); else return total;
  if(word[4]!='\0') total += enumerateChar(word[4]); else return total;
  return total;
}

/* Manipulates Filebuffer struct, returns tokenized words, new doc symbols, or null */
/* '<DOC>' and '</DOC>' indicate start and stop. 
/*TODO reduce branching*/
inline unsigned char* getNextWord(unsigned char *len,int i,int *doc){	
  while(fileBuffer[i].curr<fileBuffer[i].end){
    char c;
    unsigned long a,b;
    enum tag_e {FALSE,TRUE} tag = FALSE;
    c = fileBuffer[i].curr[0];
    if((c>=65 && c<=90) || (c>=97 && c<=122)||(c>=48 && c<=57)){
      fileBuffer[i].curr[0] = tolower(c);
      fileBuffer[i].curr++;
    }
    else if(c=='<'){
      if(fileBuffer[i].curr[1]=='D' && fileBuffer[i].curr[2]=='O' &&
	 fileBuffer[i].curr[3]=='C' && fileBuffer[i].curr[4]=='N'){
	fileBuffer[i].wordPointer=fileBuffer[i].curr=fileBuffer[i].curr+11;
	sscanf(fileBuffer[i].wordPointer,"%lu-%lu",&a,&b);
	*doc = a * 1000 + b;
	while(fileBuffer[i].curr[0]!='>')fileBuffer[i].curr++;
	fileBuffer[i].wordPointer=fileBuffer[i].curr=fileBuffer[i].curr+1;
	return magicChar;
      }
      else{
	do{fileBuffer[i].curr++;}while(fileBuffer[i].curr[0]!='>');
	fileBuffer[i].wordPointer=++fileBuffer[i].curr;
      }
      }
    else{
      unsigned char* result = fileBuffer[i].wordPointer;
      fileBuffer[i].curr[0]='\0';
      fileBuffer[i].curr++;
      fileBuffer[i].wordPointer=fileBuffer[i].curr;
      if((fileBuffer[i].curr-1)-result > 1) {
		*len = fileBuffer[i].curr-result-1;
		return result;
      }
    } 
  }
  return NULL;
}

/* Loads filebuffer structs */
void readFile(char *filename){
  FILE *fp = fopen(filename,"r");
  int res,i;
  fstat(fileno(fp), &fileBuffer[0].buf);
  fileBuffer[0].store = nextmem;
  res=fread(fileBuffer[0].store,1,fileBuffer[0].buf.st_size,fp);
  fileBuffer[0].curr = fileBuffer[0].wordPointer=fileBuffer[0].store;
  nextmem+=fileBuffer[0].buf.st_size;
  fileBuffer[0].end = fileBuffer[0].store+fileBuffer[0].buf.st_size;
  fclose(fp);
  for(i=1;i<NUMTHREADS;i++){
  	memcpy(&fileBuffer[i],&fileBuffer[0],sizeof (struct fileBufferRec));
  }
  for(i=0;i<NUMTHREADS;i++){
  	fileBuffer[i].curr = fileBuffer[0].store+i*fileBuffer[0].buf.st_size/NUMTHREADS;
  	while(strncmp(fileBuffer[i].curr,"<DOC>",5) !=0)fileBuffer[i].curr++;
  	fileBuffer[i].wordPointer= fileBuffer[i].curr;
 	fileBuffer[i].end = fileBuffer[i].store+ (i+1)*fileBuffer[i].buf.st_size/NUMTHREADS;
 	while(strncmp(fileBuffer[i].end,"</DOC>",6) !=0 && fileBuffer[i].end < fileBuffer[NUMTHREADS-1].end)fileBuffer[i].end++;
  }
}



/* Main loop of thread, fills tree with words */
void *readLoop(void *threadno){	
  unsigned char *word,len=0;
  unsigned int docno=0,i=0,uniq=0,hashval;
	void * count = requestMemory(sizeof(unsigned int));
	*((unsigned int*)count) = 0;
  while(word=getNextWord(&len,*((unsigned int*)threadno),&docno)){
    if(word[0]!='#'){
      hashval = enumerateWord(word);
		if(!mutexes[hashval]) {
			createNewMutex(hashval);			
		}
		pthread_mutex_lock(mutexes[hashval] );
      	insert(hashval,word,len+1,docno);   
      	pthread_mutex_unlock(mutexes[hashval] );
      *((unsigned int*)count) = *((unsigned int*)count) + 1;      
      //printf("%d\n",*((unsigned int*)count));
    }
   }
   pthread_exit(count);
}

/*****************************************************************/
/* Entry Point                                                   */
/*                                                               */
/*****************************************************************/
int main(int argc, char **argv){
	pthread_t threads[NUMTHREADS];
	unsigned int *threadno[NUMTHREADS];
	int i, count=0;
	void *result;
	 if(argc < 2){
  		fprintf(stderr,"Please specify an input file\n");
  		exit(1);
  	}
  	mem = malloc(TOTALMEMORY);
  	hash = (struct Node**)mem;
  	nextmem = mem + HASHSIZE*sizeof (struct Node*);
  	mutexes = (pthread_mutex_t**)requestMemory(HASHSIZE *sizeof *mutexes);
  	for(i=0;i<HASHSIZE;i++){
  		hash[i]=0;
  		mutexes[i]=0;
  	}

  	readFile(argv[1]);
  
  for(i=0;i<NUMTHREADS;i++){
  	threadno[i]=(unsigned int *)requestMemory(sizeof(unsigned int));
  	*threadno[i]= i;
  	pthread_create(&threads[i],NULL,readLoop,(void*)threadno[i]);
  }
  for(i=0;i<NUMTHREADS;i++){
    pthread_join( threads[i],&result);  
    count+=*((unsigned int*)result);
  }

  serializeAll();

  printf("\nMemory: %ld\n",TOTALMEMORY-(nextmem-mem));
  printf("Total words: %d\n",count);
  printf("Unique words: %d\n",totalEntries);
  //free(mem); //let OS handle it
  return 0;
}
