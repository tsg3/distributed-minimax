#ifndef EVALUATION_H_
#define EVALUATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

time_t time_elapsed;

void start_time();
void end_time();
double get_time_in_double();

#endif