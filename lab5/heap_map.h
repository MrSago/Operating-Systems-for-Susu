
#ifndef _HEAP_MAP_H
#define _HEAP_MAP_H

#include <stddef.h>

typedef struct __heapmap HeapMap;

HeapMap* InitDefaultHeapMap();
HeapMap* CreateHeapMap();

void InsertDataInHeapMap(HeapMap* hm, void* data, size_t data_size);
void ClearHeapMap(HeapMap* hm);
void FreeHeapMap(HeapMap** hm_ptr);

void** GetDataPointers(HeapMap* hm);
size_t GetCountPointers(HeapMap* hm);

#endif //_HEAP_MAP_H
