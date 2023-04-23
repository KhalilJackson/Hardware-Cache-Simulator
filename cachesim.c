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


/* Files can be opened using the fopen function and closed using the fclose function.
use fgets (to read single line) 
starts at file position 0 after opening
fgets advances 
calling fgets multiple itmes yields diff results (file pointer advances)

use sscanf to read input from string
you should use the format specifier %llx

only read in lines M, L, S (which have one space in front)
no space in front of I line, make sure to ignore
*/


/* HELPER FUNCTION 1 */
/* read in the line and store the bits */ 
fp = fopen("t2.trace", "r");
char  line[100]; 

//if line read sucessfully, pionter to buffer (line)
//if end of file, fgets is null

typedef unsigned long long memaddr_t; 

while (fgets(line, 100, fp)) {
if  (line[0] =  ' ') {
	sscanf(line," char %llx, int", char* space, char* op, memaddr_t* addr, int* data);
printf(*op);
printf(*addr);
printf*data); 
}
//assign offset/index/tag  bits
memaddr_t offset = *addr  << (64-b) >> (64+b); 
memaddr_t  index = *addr & (1 << (s+b)) >> b;
memaddr_t tag = index >> s; 
}


/* HELPER  */
/* need to malloc space for the cache set arrays and
need to mallco space for the cache */

//make a strcut for your cache line
//cache setes are arrays of cahce lines
//caches are 2d arrays (cache sets of cache lines)  
//create array sizes based on the s, e b values passed in by user

//QUESTION: number of tag bits depends on SEB so what type do we make it?  
struct cacheLine {
        memaddr_t tag;
        int v;
	int accessed; 
}

struct cacheLine* cacheSet = malloc(sizeof(struct cacheLine)) * E; 
struct cacheLine** cache = malloc(sizeof(struct cacheLine*)) * S; 
 





/*
line = line + 1;
char* op = line;
if (*op == ' ') {
	continue; //ignore because it is an "I" line
} else {
line = line + 2; 
typedef unsigned long long memaddr_t; 
memmaddr_t addr  = *line; 
line = line + 2; 
int size = line; 

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
}


//WAIT SHIT U DIDN'T THINK ABOUT THE  L, S, M STUFF SIGH 
//HOW DO THINGS CHANGE ACCORDINGLY 
}



}


  /**********************************************************************
   * TODO: Add code to run the cache simulation here. Make sure to modularize
   * using helper functions; don't write the rest of the program inside main!
   **********************************************************************/


  /* Output the final cache statistics */
  printSummary(hit_count, miss_count, eviction_count);
  return 0;
}

