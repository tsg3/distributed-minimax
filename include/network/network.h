#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

#include <mpi.h>

#define BIT_SET(n, b) ((n) |= (1ULL << (b)))
#define BIT_CLEAR(n, b) ((n) &= ~(1ULL << (b)))
#define BIT_CHECK(n, b) (!!((n >> b) & 1ULL))

typedef struct response
{
    int result;
    struct response* next;
} response;

response* responses_list;

void add_response(int);

char node_id;
int process_rank, cluster_size;

char worker_status;
int worker_ranks[3];

char *request_buffer, *response_buffer;
int request_len, response_len;

bool network_initialized;
bool finished_main;
char worker_state;

int init_network(int, char**);
void free_network();

pthread_t network_thread_id;
pthread_t minimax_thread_id;
pthread_mutex_t lock;

void* master_network_main();
void* worker_network_main();

void* master_minimax_main();
void* worker_minimax_main();

int worker_available(bool);
void request_worker(int, int);

#endif