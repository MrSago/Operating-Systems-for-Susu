
#include <stdio.h>

#include "heap_map.h"
#include "tools/read_input.h"

typedef struct {
    char name[255];
    int age;
} Data;

void InputDataInHeap(HeapMap* hm) {
    printf("\nEnter data count: ");
    size_t data_count = read_digit_ull();
    fflush(stdin);

    size_t pointers_count = GetCountPointers(hm);
    for (size_t i = pointers_count; i < pointers_count + data_count; ++i) {
        Data data;
        printf("# %llu\n", i + 1);

        printf("Name: ");
        fgets(data.name, 255, stdin);
        data.name[strlen(data.name) - 1] = '\0';

        printf("Age: ");
        data.age = read_digit();
        putchar('\n');

        InsertDataInHeapMap(hm, &data, sizeof(Data));
    }
}

void PrintDataFromHeap(HeapMap* hm) {
    size_t count = GetCountPointers(hm);
    if (count == 0) {
        printf("\nHeap is empty!\n");
        return;
    }

    printf("\nData from heap:\n");
    void** pointers = GetDataPointers(hm);
    for (size_t i = 0; i < count; ++i) {
        Data* data = (Data*)(pointers[i]);
        printf("# %llu\n", i + 1);
        printf("Name: %s\n", data->name);
        printf("Age: %d\n\n", data->age);
    }
}

int main() {
    HeapMap* default_heap = InitDefaultHeapMap();
    HeapMap* new_heap = CreateHeapMap();

    while (1) {
        putchar('\n');
        printf(
            "1. Insert data in default heap\n"
            "2. Print data from default heap\n"
            "3. Clear default heap\n"
            "4. Insert data in new heap\n"
            "5. Print data from new heap\n"
            "6. Clear new heap\n");

        fflush(stdin);
        switch (read_digit()) {
            case 1:
                InputDataInHeap(default_heap);
                break;

            case 2:
                PrintDataFromHeap(default_heap);
                break;

            case 3:
                ClearHeapMap(default_heap);
                printf("\nHeap is cleared!\n");
                break;

            case 4:
                InputDataInHeap(new_heap);
                break;

            case 5:
                PrintDataFromHeap(new_heap);
                break;

            case 6:
                ClearHeapMap(new_heap);
                printf("\nHeap is cleared!\n");
                break;

            case 0:
                FreeHeapMap(&new_heap);
                FreeHeapMap(&default_heap);
                printf("\nSuccess exited!\n");
                return 0;

            default:
                printf("\nUnknown option!\n");
                break;
        }
    }

    return 0;
}
