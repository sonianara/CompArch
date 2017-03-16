
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


//#define AMAX 10     /* Maximum (square) array size */
#define AMAX 2
#define CACHESIM 1    /* Set to 1 if simulating Cache */
#define CONTINUOUS 0
#define QUICKRUN 1
#define MATRIX_SIZE 2
//#define MATRIX_SIZE 10
//#define CACHE_SIZE 16
#define CACHE_SIZE 256
//#define ASSOCIATIVITY_LEVEL 1
//#define ASSOCIATIVITY_LEVEL 2
#define ASSOCIATIVITY_LEVEL 4

#include "header.h"

int is64Bit;
int numIndexBits;
int numBlockOffsetBits;
int numTagBits;
Cache cache;

static int a[AMAX][AMAX], b[AMAX][AMAX], mult[AMAX][AMAX];

void printCache() {
  int i;
  int j;
  CacheLine *cl;
  printf("Index\tV\tTag\tData\n-------------------------------\n");
  for (i = 0; i < CACHE_SIZE; i++) {
    printf("0x%X", i);
    for (j = 0; j < ASSOCIATIVITY_LEVEL; j++) {
      cl = &cache.entries[i].lines[j];
      printf("%s%d\t0x%X\t0x%X", j==0 ?"\t":"\n\t", cl->valid, cl->tag, cl->data);
    }

    printf("%s", ASSOCIATIVITY_LEVEL > 1 ? "\n\n" : "\n");
  }
}

int chooseReplacement(CacheEntry *ce) {
  int idx;
  CacheLine *cl;
  for (idx = 0; idx < ASSOCIATIVITY_LEVEL; idx++) {
    cl = &ce->lines[idx];
    if (!cl->valid) {
      //printf("Choosing replacement. Chose next: %d\n", idx);
      return idx;
    }
  }
  int replaceIndex = rand() % ASSOCIATIVITY_LEVEL;
  //printf("Choosing replacement. Chose random: %d\n", replaceIndex);
  return replaceIndex;
}

int decodeAddress(int *mp, VirtualAddress *addr) {
  unsigned int address = (uintptr_t) mp;
  addr->tag = address >> (numIndexBits + numBlockOffsetBits);
  addr->index = (address << (numTagBits)) >> (numTagBits + numBlockOffsetBits);
  addr->byteOffset = (address << (numTagBits + numIndexBits)) >> (numIndexBits + numTagBits);
  //printf("Addr: 0x%X  |  Tag: 0x%X  |  Indx: 0x%X  |  bo: 0x%X\n", address, addr->tag, addr->index, addr->byteOffset);
}

/* This function gets called with each "read" reference to memory */
int mem_read(int *mp) {
  cache.reads++;
  int line;
  int isHit = FALSE;
  VirtualAddress virtualAddress;
  CacheEntry *cacheEntry;
  CacheLine *cacheLine;
  //printf("Memory read from location %p\n", mp);
  decodeAddress(mp, &virtualAddress);
  cacheEntry = &(cache.entries[virtualAddress.index]); // Get cache entry from index
  // Search through each cache line (if there's more than one)
  for (line = 0; line < ASSOCIATIVITY_LEVEL; line++) {
    cacheLine = &(cacheEntry->lines[line]);
    if (cacheLine->valid && cacheLine->tag == virtualAddress.tag) {
      // HIT! read data from cache
      cache.hits++;
      isHit = TRUE;
      //printf("*HIT*\n");
      break;
    } 
  }
  if (!isHit) {
    // Cache line is not valid from mem 
    // and store in cache
    //printf("*MISS*\n");
    cache.misses++;
    cache.writes++;
    int idx = chooseReplacement(cacheEntry);
    cacheLine = &(cacheEntry->lines[idx]);
    cacheLine->data = *mp;
    cacheLine->tag = virtualAddress.tag;
    cacheLine->modified = TRUE;
    cacheLine->valid = TRUE;
  }
}

/* This function gets called with each "write" reference to memory */
int mem_write(int *mp) {
  //printf("Memory write to location %p\n", mp);
  cache.writes++;
  VirtualAddress virtualAddress;
  CacheEntry cacheEntry;
  CacheLine cacheLine;
  decodeAddress(mp, &virtualAddress);
  cacheEntry = cache.entries[virtualAddress.index];
  int writeIndex = chooseReplacement(&cacheEntry);
  cacheLine = cacheEntry.lines[writeIndex];
  cacheLine.tag = virtualAddress.tag;
  cacheLine.data = *mp;
  cacheLine.valid = TRUE;
  cacheLine.modified = TRUE;
  //printf("\n\n");
}

/* Statically define the arrays a, b, and mult, where mult will become the cross product of a and b, i.e., a x b. */

void matmul(int r1, int c1, int c2) {
  int i, j, k;
  int *mp1, *mp2, *mp3;

  /* Initializing elements of matrix mult to 0.*/
  for(i = 0; i < r1; ++i) {
    for(j = 0; j < c2; ++j) {
      mult[i][j] = 0;
    }
  }

  /* Multiplying matrix a and b and storing in array mult. */
  for(i = 0; i < r1; ++i) {
    for(j = 0; j < c2; ++j) {
      for(k = 0; k < c1; ++k) {

#if CACHESIM    /* "Hooks" to measure memory references - enabled if CACHESIM  */
        mp1 = &mult[i][j];
        mp2 = &a[i][k];
        mp3 = &b[k][j];   
        mem_read(mp1);
        mem_read(mp2);
        mem_read(mp3);
        mem_write(mp1); 
#endif

        //        printf("mult[i][j] += a[i][k] * b[k][j]\n");
        //        printf("\ti=%d", i);
        //        printf("\tj=%d", j);
        //        printf("\tk=%d", k);
        //        printf("\ta[i][k]=%d", a[i][k]);
        //        printf("\tb[k][j]=%d", b[k][j]);
        //        printf("\tmult[%d][%d] += %d \n", i, j, a[i][k] * b[k][j]);
        mult[i][j] += a[i][k] * b[k][j];
        //printCache();
        //printf("======================\n");
#if CONTINUOUS
        getchar();
#endif
      }
    }
  }
}

void initCache() {
  int test = 5;
  is64Bit = sizeof &test == 8;
  numIndexBits = ceil(log2(CACHE_SIZE));
  numBlockOffsetBits = 4;
  if (is64Bit) {
    printf("Running on a 64-bit machine.\n");
    numTagBits = 64 - numIndexBits - numBlockOffsetBits;
  } else {
    printf("Running on a 32-bit machine.\n");
    numTagBits = 32 - numIndexBits - numBlockOffsetBits;
  }
  printf("Cache Size: %d\n", CACHE_SIZE);
  printf("Number of Index Bits: %d\n", numIndexBits);
  printf("Number of Block Offset Bits: %d\n", numBlockOffsetBits);
  printf("Number of Tag Bits: %d\n", numTagBits);


  cache.hits = 0;
  cache.misses = 0;
  cache.reads = 0;
  cache.writes = 0;
  int num = 0;
  int entry;
  int line;
  CacheEntry cacheEntry;
  CacheLine cacheLine;
  for (entry = 0; entry < CACHE_SIZE; entry++) {
    cacheEntry = cache.entries[entry];
    for (line = 0; line < ASSOCIATIVITY_LEVEL; line++) {
      cacheLine = cacheEntry.lines[line];
      cacheLine.valid = FALSE;
      cacheLine.modified = FALSE;
      num++;
    }
  }
  printf("num: %d\n", num);
  printf("\n");
}

void printMatrix(int array, int r1, int c1, int c2) {
  /* Displaying the multiplication of two matrix. */
  int i, j;
  printf("----------\n");
  for(i = 0; i < r1; ++i) {
    for(j = 0; j < c2; ++j) {
      if (array == MATRIX_A) {
        printf("%04d  ", a[i][j]);
      } else if (array == MATRIX_B) {
        printf("%04d  ", b[i][j]);
      } else if (array == MATRIX_MULT) {
        printf("%04d  ", mult[i][j]);
      }
      if(j == c2 - 1) {
        printf("\n");
      }
    }
  }
}

int main() {
  srand(time(NULL));
  initCache();

  int r1, c1, r2, c2, i, j, k;

  int *mp1, *mp2, *mp3;

#if QUICKRUN
  r1 = MATRIX_SIZE;
  c1 = MATRIX_SIZE;
  r2 = MATRIX_SIZE;
  c2 = MATRIX_SIZE;
#else
  printf("Enter rows and column for first matrix: ");
  scanf("%d%d", &r1, &c1);

  printf("Enter rows and column for second matrix: ");
  scanf("%d%d", &r2, &c2);
#endif

  /* If column of first matrix in not equal to row of second matrix, asking user to enter the size of matrix again. */
  while (c1 != r2) {
    printf("Error! column of first matrix not equal to row of second.\n");
    printf("Enter rows and column for first matrix: ");
    scanf("%d%d", &r1, &c1);
    printf("Enter rows and column for second matrix: ");
    scanf("%d%d", &r2, &c2);
  }

  /* Storing elements of first matrix. */
  printf("\nEnter elements of matrix 1:\n");
  for(i = 0; i < r1; ++i) {
    for(j = 0; j < c1; ++j) {
#if QUICKRUN
      a[i][j] = i + j; // build sample data
#else
      printf("Enter elements a%d%d: ", i + 1, j + 1);
      scanf("%d", &a[i][j]);
#endif
    }
  }

  /* Storing elements of second matrix. */
  printf("\nEnter elements of matrix 2:\n");
  for(i = 0; i < r2; ++i) {
    for(j = 0; j < c2; ++j) {
#if QUICKRUN
      b[i][j] = 10 + i + j;
#else
      printf("Enter elements b%d%d: ", i + 1, j + 1);
      scanf("%d", &b[i][j]);
      getchar();
#endif
    }
  }


  printf("\nInput Matrix A:\n");
  printMatrix(MATRIX_A, r1, c1, c2);
  printf("\nInput Matrix B:\n");
  printMatrix(MATRIX_B, r1, c1, c2);

  matmul(r1, c1, c2);    /* Invoke matrix multiply function */ 


  /* Displaying the multiplication of two matrix. */
  printf("\nOutput Matrix:\n");
  printMatrix(MATRIX_MULT, r1, c1, c2);
  printf("Num of reads: %d\n", cache.reads);
  printf("Num of writes: %d\n", cache.writes);
  printf("Num of hits: %d\n", cache.hits);
  printf("Number of misses: %d\n", cache.misses);
  double hitRate = (cache.hits * 100) / (cache.hits + cache.misses);
  printf("Hit Rate: %lf\n", hitRate);
  return 0;
}


