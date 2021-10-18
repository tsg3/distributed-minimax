#ifndef EVALUATION_H_
#define EVALUATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef struct Measure
{
    double value;
    double time;
    struct Measure* next;
} Measure;

time_t time_elapsed;
Measure* CPU_list;
Measure* RAM_list;
char* stat_path;
char* statm_path;

void init_evaluation_module();

void start_time();
time_t get_current_time();
void end_time();
double get_time_in_double();

FILE* open_file(char*);

void get_cpu_usage();
void get_ram_usage();

void add_measure(Measure**, double, double);
void free_lists();

#endif