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
    struct CPU_measure* next;
} Measure;

time_t time_elapsed;
Measure* CPU_list;

void init_evaluation_module();

void start_time();
time_t get_current_time();
void end_time();
double get_time_in_double();

void get_cpu_usage();
void add_cpu_measure(double, double);
void free_cpu_list();

#endif