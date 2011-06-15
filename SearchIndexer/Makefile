all: makeIndex search makeIThread generator

makeIndex: makeIndex.c types.h
	gcc -O3 makeIndex.c -o makeIndex

search: search.c types.h
	gcc -O3 search.c -o search 
	
makeIThread: makeIThread.c types.h
	gcc -O3 makeIThread.c -lpthread -o makeIThread
	
generator: generator.c 
	gcc -O3 generator.c -o generator -lm