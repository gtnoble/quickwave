#ifndef QUICKWAVE_MEMORY
#define QUICKWAVE_MEMORY

#include <stdlib.h>

typedef void *Allocator(size_t);
typedef void Deallocator(void *);

typedef struct {
    Allocator *allocate;
    Deallocator *deallocate;
} MemoryManager;

#endif