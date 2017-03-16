
#ifndef CACHE_SIZE
#define CACHE_SIZE
#endif
#ifndef ASSOCIATIVITY_LEVEL
#define ASSOCIATIVITY_LEVEL
#endif
#define TRUE 1
#define FALSE 0

#define MATRIX_A 3
#define MATRIX_B 4
#define MATRIX_MULT 5

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
  CacheLine lines[ASSOCIATIVITY_LEVEL];
} CacheEntry;


typedef struct Cache {
  CacheEntry entries[CACHE_SIZE];
  int hits;
  int misses;
  int reads;
  int writes;
} Cache;



