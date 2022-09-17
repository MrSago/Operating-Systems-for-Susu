
#include <mem.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUF_SIZE (10)

typedef struct {
    pthread_mutex_t mtx;
    pthread_cond_t cv;
    bool done;
    char* buf;
    size_t count;
} shared_state_t;

void shared_state_init(shared_state_t* shared_state) {
    pthread_mutex_init(&shared_state->mtx, NULL);
    pthread_cond_init(&shared_state->cv, NULL);
    shared_state->done = false;
    shared_state->buf = (char*)malloc(BUF_SIZE);
    memset(shared_state->buf, '\0', BUF_SIZE);
    shared_state->count = 0;
}

void shared_state_destroy(shared_state_t* shared_state) {
    pthread_mutex_destroy(&shared_state->mtx);
    pthread_cond_destroy(&shared_state->cv);
    free(shared_state->buf);
}

void* generate_string(void* arg) {
    shared_state_t* ss = (shared_state_t*)arg;
    srand(time(NULL));
    for (;;) {
        pthread_mutex_lock(&ss->mtx);
        while (ss->done) {
            pthread_cond_wait(&ss->cv, &ss->mtx);
        }
        for (int i = 0; i < BUF_SIZE - 1; ++i) {
            ss->buf[i] = 'a' + rand() % ('z' - 'a');
        }
        ss->count += 1;
        ss->done = true;
        pthread_cond_signal(&ss->cv);
        pthread_mutex_unlock(&ss->mtx);
    }
    return NULL;
}

void* print_string(void* arg) {
    shared_state_t* ss = (shared_state_t*)arg;
    for (;;) {
        pthread_mutex_lock(&ss->mtx);
        while (!ss->done) {
            pthread_cond_wait(&ss->cv, &ss->mtx);
        }
        printf("%lu : %s\n", ss->count, ss->buf);
        ss->done = false;
        pthread_cond_signal(&ss->cv);
        pthread_mutex_unlock(&ss->mtx);
    }
    return NULL;
}

int main(int argc, char** argv) {
    shared_state_t shared_state;
    shared_state_init(&shared_state);

    pthread_t thread_generate;
    pthread_t thread_print;

    int result_generate =
        pthread_create(&thread_generate, NULL, generate_string, &shared_state);
    int result_print =
        pthread_create(&thread_print, NULL, print_string, &shared_state);
    if (result_generate || result_print) {
        printf("The threads could not be created.\n");
        exit(1);
    }

    result_generate = pthread_join(thread_generate, NULL);
    result_print = pthread_join(thread_print, NULL);
    if (result_generate || result_print) {
        printf("The threads could not be joined.\n");
        exit(2);
    }

    shared_state_destroy(&shared_state);
    return 0;
}
