OVERVIEW:
	Four programs:
		makeIndex - builds an index from a specified input file.  Single threaded, but fastest.
		makeIThread - does the same but is threaded (10 threads).
		search - searches the index, queries read from stdin.
		generator - generates ~100MB text file from a 3rd order HMM, made from stdin input text.
		
		

USAGE:
	compile with 'make', requires C99 compiler (I used gcc 4.2.1).  For C90, just drop all the 'inline' keywords.
	make test data:
		./generator < alice.txt > text.txt
	
	then index data:
		./makeIndex text.txt
		
	and search:
		./search
		
	followed by queries from stdin.
	e.g.
	
		Ready for queries:
		the
		the is on index page 303 after entry thaterillarmles
		the found 68577 times in 0.001215 seconds.


NOTES:
	Out lecturer, Andrew Trotman, told us the way to make a REALLY fast index was to break
	a lot the rules of good practice.  So you will see quite a bit of it here...
		- unaligned memory (everything is a unsigned char*).  I could fix this (for a memory hit), and I could get a speed up.
		- Deferencing void pointers (but obfuscating it so the compiler doesn't complain).
		- The memory usage is very high (~1.7 GBs), and I didn't introduce a dynamic reallocation.  (trivial, but expensive).
			Change the directive at the top of the index source to reduce memory use.
		- I couldn't get gcc to flatten out my recursive tree functions, so I did it myself, resulting a in a loss of readability.
		- I don't deallocate.... the programs life span is supposed to be as short as possible, so I just let the OS handle it.
		
	Also:
		- search is done in two reads, one from index.dat (which is binary searched), then one for the correct page in serial.dat
		- If I get a free Sunday, I think I will remove the shared data structs, instead having individual ones that are merged
			at the end.  Contention hurts.
		- GCC has keywords to prevent caching.  This would be handy for long words, and perhaps only caching the most frequently 
			accessed nodes.
		- I haven't really commented any of this, as I wrote it for myself.  If you would like actual documentation, I would be
			happy to whip something up.
		- Once the file is memory mapped, run time is MUCH faster.  So run it twice.
		- makeIThread is pretty much untested, and I have had it seg fault unexpectantly.  I will fix this when I get a chance.	
		- Actually, I think I broken the threaded version, as it is VERY slow atm.  Consider it work in progress.