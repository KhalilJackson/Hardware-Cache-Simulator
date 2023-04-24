/*
 * Lab 5 - Make Some Cache
 *
 * <Khalil Jackson, Diyaa Yaqub>
 *
 * cachesim.c - source file for your hardware cache simulator.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <getopt.h>
#include <math.h>

/*  maintains LRU */ 
int lruCounter = 0; 


/* Globals set by command line arguments */
int verbose = 0; /* whether to print verbose output */
int s = 0; /* set index bits */
int b = 0; /* block offset bits */
int E = 0; /* lines per set */
char* trace_file = NULL; /* input trace filename */

/* Derived from command line args */
int S; /* number of sets */
int B; /* block size (bytes) */

/* Counters used to record cache statistics */
int miss_count = 0;
int hit_count = 0;
int eviction_count = 0;

/* 
 * Print the cache simulation statistics. The simulator must call
 * this function in order to be properly tested. Do not modify!
 */
void printSummary(int hits, int misses, int evictions) {
  printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
  FILE* output_fp = fopen(".cachesim_results", "w");
  assert(output_fp);
  fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
  fclose(output_fp);
}

/*
 * Print program usage information. Do not modify!
 */
void printUsage(char** argv) {
  printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
  printf("Options:\n");
  printf("  -h         Print this help message.\n");
  printf("  -v         Print verbose output.\n");
  printf("  -s <num>   Number of set index bits.\n");
  printf("  -E <num>   Number of lines per set.\n");
  printf("  -b <num>   Number of block offset bits.\n");
  printf("  -t <file>  Trace filename.\n");
  printf("\nExamples:\n");
  printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/t1.trace\n", argv[0]);
  printf("  linux>  %s -s 8 -E 2 -b 4 -t traces/t1.trace -v\n", argv[0]);
  exit(0);
}

/*
 * Reads command-line arguments, runs the cache simulation, and prints
 * the simulation statistics (hits, misses, and evictions) on completion.
 */
int main(int argc, char** argv) {

  /* Parse command line arguments */
  char c;
  while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
    switch (c) {
      case 's':
        s = atoi(optarg);
        break;
      case 'E':
        E = atoi(optarg);
        break;
      case 'b':
        b = atoi(optarg);
        break;
      case 't':
        trace_file = optarg;
        break;
      case 'v': // turn on verbose output
        verbose = 1;
        break;
      case 'h': // print the program help message
        printUsage(argv);
        exit(0);
      default: // unrecognized option; print the help message
        printUsage(argv);
        exit(1);
    }
}

  /* Make sure that all required command line args were specified */
  if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
    printf("%s: Missing required command line argument\n", argv[0]);
    printUsage(argv);
    exit(1);
  }

  /* Compute S and B from command line args */
  S = (int) pow(2, s);
  B = (int) pow(2, b);

  /* An example of verbose output. */
  if (verbose) {
    printf("simulation starting and reading from %s\n", trace_file);
  }

/* HELPER FUNCTION 1 */
/* read in the line and store the bits */ 
FILE* fp = fopen("traces/t2.trace", "r");
char line[100]; 

//if line read sucessfully, pionter to buffer (line)
//if end of file, fgets is null

typedef unsigned long long memaddr_t; 

char* op = 0;
memaddr_t* addr = 0; 
int* data = 0; 

while (fgets(line, 100, fp)) {
if  (line[0] ==  ' ') {
	sscanf(line, " %c  %llx, %d", op, addr, data);
//	printf("%c", *op);
//	printf("%lld", *addr);
	
//
	printf("%d", *data); 
}
}


/*

//TODO: neeed to check if this works and parses
assign offset/index/tag  bits
memaddr_t offset = *addr  << (64-b) >> (64+b); 
memaddr_t  index = *addr & (1 << (s+b)) >> b;
memaddr_t tag = index >> s; 

if (op == 'M') {
	//if its a hit, increment hit count 2 
	//if its a miss, increment miss count, hit count, evict count
	//if its a miss you should also rewrite the contents tho


bool hit = false; 
//check if its a hit
struct cacheLine* lines = cacheSet[index]; 
for (in i = 0; i < E; i++) {
        struct cacheLine Sline = lines[i]; 
        //if valid bit = 0; 
        if (Sline->v == 0 && Sline-> tag == tag) {
		if (op == 'M') {
//TODO
		hit = true;
		hit_count += 2;
		} else {
	
		lruCounter++; 
		sLine 0> accessed = lruCounter; 
		hit_count +=1;
		hit = true;  
		}
	} 
}

//check if it is a miss
if (!hit) {
	miss_count += 1; 
bool toEvict = true; 
struct cacheLine lru = lines[0];  
//setting a line in the cache
	for (in i = 0; i < E; i++) {
		struct cacheLine Sline = lines[i]; 
		if (Sline-> v == 0) { //there is space in set
		if (op == M) {
//TODO
			hit_count +=1;
		} else {
			lru->v = 1; 
			lru->tag  = tag; 
			lruCounter++;                         
			Sline0-> accessed = lruCounter;
			toEvict = false;  
		}
		//finding lru 
		if (Sline-> accessed < lru->accessed) {
			lru = Sline; 
		}
	`}
}

//evict LRU by replacing metadata
if (toEvict) {

	evict_count += 1; 
	lru->v = 1; 
	lru->tag  = tag; 
	lruCounter++;                         
	Sline0-> accessed = lruCounter; 
}	

}
*/
/* HELPER */  
/* need to malloc space for the cache set arrays and
need to mallco space for the cache */

//make a strcut for your cache line
//cache setes are arrays of cahce lines
//caches are 2d arrays (cache sets of cache lines)  
//create array sizes based on the s, e b values passed in by user

/* 
//QUESTION: number of tag bits depends on SEB so what type do we make it?  
struct cacheLine {
        memaddr_t tag;
        int v;
	int accessed; 
}



struct cacheLine** cache = malloc(sizeof(struct cacheLine*)) * S; 
for (int i = 0; i < S; i++) {
	struct cacheLine* cacheSet = malloc(sizeof(struct cacheLine)) * E; 
	for(int j = 0; j < E; j++) {
        	struct cachLine* line; 
        	line->v = 0; 
        	cacheSet[j] = line; 
	}
	cache[i] = cacheSet;
}


	
*/






//now we need to set the bits
//use the addr, s and b to do this





/* HELPER 3 */
//based on calculated tag, and s bits, go into the cache 
//at s index, go through the lines in the set array and try to match tag
//if none match: miss ++ AND create cache line (HELPER 4)

//if match: hit ++; 


/* HELPER 4 */
//create a cacheline type and assign v and tag value based on calculated
//add this to the cache set array if there is space

//if there is no space, evict ++ 
//figure out how to evict the LEAST RECENT USED
//add it

//put the cache set in teh correct spot in the cache array 

//WAIT SHIT U DIDN'T THINK ABOUT THE  L, S, M STUFF SIGH 
//HOW DO THINGS CHANGE ACCORDINGLY 


return 0; 
}


  /**********************************************************************
   * TODO: Add code to run the cache simulation here. Make sure to modularize
   * using helper functions; don't write the rest of the program inside main!
   **********************************************************************/


  /* Output the final cache statistics */
//  printSummary(hit_count, miss_count, eviction_count);
 // return 0;
