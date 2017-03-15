

#define CACHE_SIZE 16
#define ASSOCIATIVITY 1

typedef struct CacheLine {
  int valid;
  int modified;
  int tag;
  int data;
} CacheLine;

typedef struct CacheEntry {
  CacheLine blocks[ASSOCIATIVITY];
} CacheEntry;


typedef struct Cache {
  CacheEntry entry[CACHE_SIZE];
} Cache;


