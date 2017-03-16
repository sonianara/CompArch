
#include <stdio.h>
#include <math.h>

#define AMAX 10     /* Maximum (square) array size */
#define CACHESIM 0    /* Set to 1 if simulating Cache */

//#define CACHE_SIZE 16
#define CACHE_SIZE 256
#define ASSOCIATIVITY_LEVEL 1
#define TRUE 1
#define FALSE 0

#define MATRIX_A 3
#define MATRIX_B 4
#define MATRIX_MULT 5

int is64Bit;
int numIndexBits;
int numBlockOffsetBits;
int numTagBits;
Cache cache;

static int a[AMAX][AMAX], b[AMAX][AMAX], mult[AMAX][AMAX];

typedef struct VirtualAddress {
  unsigned int tag;
  unsigned int index;
  unsigned int byteOffset;
} VirtualAddress;

typedef struct CacheLine {
  unsigned int valid;
  unsigned int modified;
  unsigned int tag;
  unsigned int data;
} CacheLine;

typedef struct CacheEntry {
  CacheLine blocks[ASSOCIATIVITY_LEVEL];
} CacheEntry;


typedef struct Cache {
  CacheEntry entry[CACHE_SIZE];
} Cache;

int decodeAddress(int *mp, VirtualAddress *addr) {
  unsigned int address = mp;
  addr->tag = address >> (numIndexBits + numBlockOffsetBits);
  addr->index = (address << (numTagBits)) >> (numTagBits + numBlockOffsetBits);
  addr->byteOffset = (address << (numTagBits + numIndexBits)) >> (numIndexBits + numTagBits);
  printf("Address: 0x%X\n", mp);
  printf("Tag: 0x%X\n", addr->tag);
  printf("Index: 0x%X\n", addr->index);
  printf("byteOffset: 0x%X\n", addr->byteOffset);
}

/* This function gets called with each "read" reference to memory */
mem_read(int *mp) {
  int idx;
  VirtualAddress va;
  decodeAddress(mp, &va);
  // Search each cache bucket
  CacheEntry ce = cache[]
    /// holla here
  for (idx = 0; idx < ASSOCIATIVITY_LEVEL; idx++) {
    
  }

  /* printf("Memory read from location %p\n", mp);  */
}

/* This function gets called with each "write" reference to memory */
mem_write(int *mp) {
  /* printf("Memory write to location %p\n", mp); */
}

/* Statically define the arrays a, b, and mult, where mult will become the cross product of a and b, i.e., a x b. */

void matmul( r1, c1, c2 ) {
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
  printf("\n");
}

void printMatrix(int array, int r1, int c1, int c2) {
  /* Displaying the multiplication of two matrix. */
  int i, j;
  printf("----------\n");
  for(i = 0; i < r1; ++i) {
    for(j = 0; j < c2; ++j) {
      if (array == MATRIX_A) {
        printf("%d  ", a[i][j]);
      } else if (array == MATRIX_B) {
        printf("%d  ", b[i][j]);
      } else if (array == MATRIX_MULT) {
        printf("%d  ", mult[i][j]);
      }
      if(j == c2 - 1) {
        printf("\n\n");
      }
    }
  }
}

int main() {
  initCache();

  int r1, c1, r2, c2, i, j, k;

  int *mp1, *mp2, *mp3;

  printf("Enter rows and column for first matrix: ");
  scanf("%d%d", &r1, &c1);
  printf("Enter rows and column for second matrix: ");
  scanf("%d%d", &r2, &c2);

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
      printf("Enter elements a%d%d: ", i + 1, j + 1);
      //printf("%d\n", i + j);
      //scanf("%d", &a[i][j]);
      a[i][j] = i + j; // build sample data
    }
  }

  /* Storing elements of second matrix. */
  printf("\nEnter elements of matrix 2:\n");
  for(i = 0; i < r2; ++i) {
    for(j = 0; j < c2; ++j) {
      printf("Enter elements b%d%d: ", i + 1, j + 1);
      //printf("%d\n", 10 + i + j);
      //scanf("%d", &b[i][j]);
      b[i][j] = 10 + i + j;
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
  return 0;
}


