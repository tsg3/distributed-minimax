#ifndef EVALUATION_H_
#define EVALUATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct Measure
{
    double value;
    double time;
    struct Measure* next;
} Measure;

double wall_seconds;

time_t first_time;
time_t time_elapsed;

unsigned long last_cpu_usage_times;
time_t last_time;
unsigned long hertz;
unsigned long num_processors;
unsigned long page_size;

Measure* CPU_list;
Measure* RAM_list;
char* stat_path;
char* statm_path;

clock_t cpu_time_elapsed;

pthread_t eval_thread_id;
bool eval_thread_exec;

void* eval_main();

void init_evaluation_module();

void start_time();
time_t get_current_time();
void end_time();

FILE* open_file(char*);

void get_cpu_usage(time_t);
void get_ram_usage(time_t);

void add_measure(Measure**, double, double);
void free_lists();

#endif