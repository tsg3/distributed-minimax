#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <chess/chess.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <cJSON.h>

#include <mpi.h>

#define BIT_SET(n, b) ((n) |= (1ULL << (b)))
#define BIT_CLEAR(n, b) ((n) &= ~(1ULL << (b)))
#define BIT_CHECK(n, b) (!!((n >> b) & 1ULL))

typedef struct response
{
    int result;
    int level;
    struct response* next;
} response;

response* responses_list;

char worker_status;
int worker_ranks[3];

pthread_mutex_t lock;

char* data_to_json(State*, int);
void json_to_data(char*, State**, int*);

char* resp_to_json(int, int);
void json_to_resp(char*, int*, int*);

int worker_available(bool);
void request_worker(State*, int, int);

/** TEST */
void test(State**, int*);

#endif