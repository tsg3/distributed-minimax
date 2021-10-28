#include <stdio.h>
#include <configuration.h>
#include <evaluation/evaluation.h>
#include <reports/reports.h>
#include <sys/time.h>

int main () 
{
    init_chess_models();
    conf = configure_minimax();

    if (conf == NULL)
    {
        clean_passant_temp();
        return -1;
    }

    struct timeval wall_start, wall_stop;

    pthread_create(&eval_thread_id, NULL, eval_main, NULL);

    cpu_time_elapsed = clock();
    gettimeofday(&wall_start, NULL);

    calc_level(conf->initial_state, 0);

    cpu_time_elapsed = clock() - cpu_time_elapsed;
    gettimeofday(&wall_stop, NULL);

    eval_thread_exec = false;
    clean_passant_temp();
    delete_configuration(conf);

    wall_seconds = (double)(wall_stop.tv_usec - wall_start.tv_usec) / 1000000
        + (double)(wall_stop.tv_sec - wall_start.tv_sec);

    pthread_join(eval_thread_id, NULL);

    export_report();

    free_lists();
    
    return 0;
}