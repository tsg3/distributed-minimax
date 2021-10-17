#include <evaluation/evaluation.h>

void start_time()
{
    time_elapsed = time(NULL);
}

void end_time()
{
    time_elapsed = time(NULL) - time_elapsed;
}

double get_time_in_double()
{
    return (double) time_elapsed;
}