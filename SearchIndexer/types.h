#ifndef TYPES_H_
#define TYPES_H_
#include <sys/stat.h>
#define MAXWORD 100

typedef struct EntryRec *Entry;

struct EntryRec{
  unsigned char *key,*hits,len;
  unsigned int *doc;
  unsigned int size;
};

struct IndexRec{
  unsigned int size;
  unsigned char **keys, *len;
  unsigned long *start;
};

struct fileBufferRec{
  FILE *fp;
  unsigned char* store, *wordPointer,*curr,*end, tag[MAXWORD];
  struct stat buf;
};


struct Node{
  unsigned char len;
  struct Node *left,*right;
  unsigned char *key,*currhit,*hits,order;
  unsigned int *docs,*currdoc,remain,count;
};



#endif