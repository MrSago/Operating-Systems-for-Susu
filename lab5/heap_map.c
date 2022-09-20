
#include "heap_map.h"

#include <Windows.h>
#include <stdio.h>

struct __heapmap {
  HANDLE heap;
  LPVOID* data_pointers;
  SIZE_T count_pointers;
};

HeapMap* InitDefaultHeapMap() {
  HeapMap* hm = (HeapMap*)malloc(sizeof(HeapMap));
  hm->heap = GetProcessHeap();
  hm->data_pointers = (LPVOID*)NULL;
  hm->count_pointers = 0;
  return hm;
}

HeapMap* CreateHeapMap() {
  HeapMap* hm = (HeapMap*)malloc(sizeof(HeapMap));
  hm->heap = HeapCreate(0, 0, 0);
  hm->data_pointers = (LPVOID*)NULL;
  hm->count_pointers = 0;
  return hm;
}

void InsertDataInHeapMap(HeapMap* hm, void* data, size_t data_size) {
  LPVOID address = HeapAlloc(hm->heap, 0, data_size);
  CopyMemory(address, data, data_size);
  hm->data_pointers = (LPVOID*)realloc(hm->data_pointers,
                                       ++hm->count_pointers * sizeof(LPVOID));
  hm->data_pointers[hm->count_pointers - 1] = address;
}

void ClearHeapMap(HeapMap* hm) {
  for (SIZE_T i = 0; i < hm->count_pointers; ++i) {
    HeapFree(hm->heap, 0, hm->data_pointers[i]);
  }
  free(hm->data_pointers);
  hm->data_pointers = (LPVOID*)NULL;
  hm->count_pointers = 0;
}

void FreeHeapMap(HeapMap** hm_ptr) {
  if (*hm_ptr == (HeapMap*)NULL) {
    return;
  }
  ClearHeapMap(*hm_ptr);
  if ((*hm_ptr)->heap != GetProcessHeap()) {
    HeapDestroy((*hm_ptr)->heap);
    free(*hm_ptr);
    *hm_ptr = (HeapMap*)NULL;
  }
}

void** GetDataPointers(HeapMap* hm) { return (void**)hm->data_pointers; }

size_t GetCountPointers(HeapMap* hm) { return (size_t)hm->count_pointers; }
