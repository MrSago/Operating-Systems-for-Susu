
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "heap_map.h"
#include "main.h"

#define UT_TEST_REJECTED (-1)
#define UT_TEST_PASS (0)
#define UT_COUNT_NOT_EQUAL (1)

typedef struct {
    Data* tests;
    size_t count;
} UnitTest;

UnitTest ParseTestsFromFile(const char path[]) {
    UnitTest parse;
    parse.tests = (Data*)NULL;
    parse.count = 0;

    FILE* file = fopen(path, "r");
    if (file == (FILE*)NULL) {
        fprintf(stderr, "Can't open file for read tests: %s\n", path);
        return parse;
    }

    char buf[255] = {'\0'};
    fgets(buf, 255, file);
    sscanf(buf, "%llu", &parse.count);

    parse.tests = (Data*)malloc(parse.count * sizeof(Data));
    if (parse.tests == (Data*)NULL) {
        fprintf(stderr, "Error allocate memory\n");
        return parse;
    }

    for (size_t i = 0; i < parse.count; ++i) {
        fgets(buf, 255, file);
        size_t len = strlen(buf);
        if (buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }
        strcpy(parse.tests[i].name, buf);

        fgets(buf, 255, file);
        sscanf(buf, "%d", &parse.tests[i].age);
    }

    fclose(file);
    return parse;
}

int CheckDataInHeapTest(const char heap_type[], HeapMap* (*init_funtion)(),
                        UnitTest* ut_ptr, FILE* log) {
    fprintf(log,
            "Heap type: %s\n"
            "Test: check data in heap\n",
            heap_type);

    HeapMap* hm = init_funtion();
    fprintf(log,
            "Init heapmap\n"
            "Insert data:\n");

    for (size_t i = 0; i < ut_ptr->count; ++i) {
        InsertDataInHeapMap(hm, ut_ptr->tests + i, sizeof(Data));
        fprintf(log,
                "\nCase # %d:\n"
                "Name: %s\n"
                "Age: %d\n",
                i + 1, ut_ptr->tests[i].name, ut_ptr->tests[i].age);
    }

    size_t count = GetCountPointers(hm);
    if (count != ut_ptr->count) {
        fprintf(log,
                "Data count in heap not equaled!\n"
                "Count pointers: %llu\n"
                "Count data unit tests: %llu\n",
                count, ut_ptr->count);
        return UT_COUNT_NOT_EQUAL;
    }

    void** pointers = GetDataPointers(hm);
    bool accepted = true;

    fprintf(log, "\nChecking data...\n");
    for (size_t i = 0; i < count; ++i) {
        fprintf(log, "Case # %d: ", i + 1);

        bool check = true;
        Data* ut_case = ut_ptr->tests + i;
        Data* heap_data = (Data*)(pointers[i]);

        if (strcmp(ut_case->name, heap_data->name)) {
            check &= false;
            fprintf(log,
                    "Name not equal!\n"
                    "Case: %s\n"
                    "Heap: %s\n",
                    ut_case->name, heap_data->name);
        }

        if (ut_case->age != heap_data->age) {
            check &= false;
            fprintf(log,
                    "Age not equal!\n"
                    "Case: %d\n"
                    "Heap: %d\n",
                    ut_case->age, heap_data->age);
        }

        fprintf(log, check ? "Accepted\n" : "Rejected\n");
        accepted &= check;
    }

    return accepted ? UT_TEST_PASS : UT_TEST_REJECTED;
}

void StartUnitTests() {
    const char log_fname[] = "log.out";
    const char test_fname[] = "test1.in";

    
    FILE* log = fopen(log_fname, "a+");
    if (log == (FILE*)NULL) {
        fprintf(stderr, "Can't open log file for writing\n");
        fclose(log);
        return;
    }

    UnitTest ut = ParseTestsFromFile(test_fname);
    if (ut.tests == (Data*)NULL || ut.tests == 0) {
        return;
    }

    printf("Test file: %s\n", test_fname);
    fprintf(log, "Test file: %s\n", test_fname);

    printf("Check data in default heap test: ");
    if (CheckDataInHeapTest("default heap", InitDefaultHeapMap, &ut, log) ==
        UT_TEST_PASS) {
        printf("passed!\n");
        fprintf(log, "\nTest passed!\n");
    } else {
        printf("failed!\n");
        fprintf(log, "\nTest failed!\n");
    }
    fprintf(log, "\n\n\n\n\n");

    printf("Check data in custom heap test: ");
    if (CheckDataInHeapTest("custom heap", CreateHeapMap, &ut, log) ==
        UT_TEST_PASS) {
        printf("passed!\n");
        fprintf(log, "\nTest passed!\n");
    } else {
        printf("failed!\n");
        fprintf(log, "\nTest failed!\n");
    }
    fprintf(log, "\n\n\n\n\n");

    fclose(log);
    free(ut.tests);
}
