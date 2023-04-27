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
#include <stdbool.h>


/* cache global var */
typedef unsigned long long memaddr_t; 
struct cacheLine {
        memaddr_t tag;
        int v;
        int accessed; 
};
struct cacheLine** cache;
 
/*  maintains LRU */ 
int lruCounter = 0; 
int lruLine = 0; 

/* Global tag, index and offset bits for a line */
memaddr_t offset;
memaddr_t index;
memaddr_t tag; 
char op; 

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
 * Creates cache data strucute 
 */
void createCache() {
  cache = malloc(sizeof(struct cacheLine*) * S);
    //initialies number of cacheSets in cache
    for (int i = 0; i < S; i++) {
      struct cacheLine* cacheSet = malloc(sizeof(struct cacheLine) * E); 
      //initialies number of cache lines in cache set
      for(int j = 0; j < E; j++) {
         struct cacheLine line; 
           line.v = 0;
           line.accessed = 0; 
           line.tag = 0; 
           cacheSet[j] = line; 
      }
      cache[i] = cacheSet;
    }
}

/* 
 * Used to break the memory address up into offset, index, and tag bits and saves to global variables  
 */ 
void setBits(memaddr_t addr) {
	memaddr_t mask = (1 << b) - 1; 
	offset = addr & mask; 
	mask = (1 << s) - 1; 
	index = (addr >> b) & mask;
	tag = addr  >> (s+b);
}

/* 
 * Loops through all lines in the cache set (based on index bits of address) and updates the accessed value
 * to mantain lru functionality if it is a hit. Returns true if it is a hit, and false if it is not a hit.   
 */
bool isHit() {
	bool hit = false; 
	struct cacheLine* lines = cache[index];
	//loop through all lines in set
	for (int i = 0; i < E; i++) {
        	//if tag matches and it valid bit is 1, it is a hit
        	if (lines[i].v == 1 && lines[i].tag == tag) {
                	lruCounter++; 
                	lines[i].accessed = lruCounter; 
                	if (op == 'M') {
                		hit_count += 2; 
               		 } else {
                        	hit_count ++;
                	}
               		hit = true; 
			return hit; 
        	}
	}
return hit; 
}

/* 
 * Updates the values of cache line in the cache cache set. It replaces the 
 * accessed value, the tag value, and the valid bit. This happens when there is a miss. 
 * The method takes in the index of the line in the set when multiple cache lines are in the set 
 * so that the correct one is replaced. The index of lru element is passed in when there is an eviction.  
 */
void setLine(int indexInSet) {
	cache[index][indexInSet].v = 1; 
	cache[index][indexInSet].tag = tag; 
	lruCounter++; 
	cache[index][indexInSet].accessed = lruCounter;
}

/*  Retuns true if there is no space in the cache set and a cache line needs to be evicted. Method returns
 * false when there is space in the set. When there is sapce in the set, the cache line is updated. When 
 * an eviction is needed, lru element is saved to the global variable and the index at which it existed
 * in the set. 
 */
bool toEvict() {
	bool toEvict = true; 
	struct cacheLine* lines = cache[index];
	
	//arbitrarily assigns lru to cache line index 0 in cache set
	lruLine = 0; 
        
	for (int i = 0; i < E; i++) {
                if (lines[i].v == 0) { 
			//there is space in set
			setLine(i);
			toEvict = false;
                        if (op == 'M') {
                                hit_count++;
                        }
			return toEvict;
                }
		//finding lru 		
		if (lines[i].v == 1) {
		 if (lines[i].accessed < lines[lruLine].accessed) {
                        lruLine = i;  
                       }
		}

        }
	return toEvict; 
}

/* 
 * Frees up allocated memory. 
 */ 
void freeAll() {
	for (int i = 0; i < S; i++) {
        	free(cache[i]);
	}
	free(cache); 
}

/* 
 * Loops through each line in the file and mantains the miss, hit, and eviction count
 * for each line (which cumulates to find the total). 
 */
void setCounts() {
	memaddr_t addr = 0; 
	FILE* fp = fopen(trace_file, "r");
	char line[100]; 
	createCache(); 

	while (fgets(line, 100, fp)) {
		if  (line[0] ==  ' ') {
        		sscanf(line, " %c  %llx,", &op, &addr);
        		setBits(addr);
		//if it is not a hit, it is a miss
		if (!isHit()) {
			miss_count ++;                 
			  
        //if it is a miss, toEvict() determines if eviction required 
        if (toEvict()) {
        //need to evict and change metadata                     
        eviction_count ++; 
          if (op == 'M') {
            hit_count++; 
          }
        setLine(lruLine);
        } 
    }
  }
  }
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

  /* Loop through each line in the file and set cache statistics */
  setCounts(); 

  /* output the  final cache statistics */
  printSummary(hit_count, miss_count, eviction_count);
  freeAll();
  return 0; 
}





