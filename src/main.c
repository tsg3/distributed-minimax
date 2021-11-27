/**
 * @file main.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Main executable of the minimax evaluation system.
 * @version 0.1
 * @date 2021-10-31
 */

#include <stdio.h>
#include <network/network.h>
#include <evaluation/evaluation.h>
#include <reports/reports.h>
#include <sys/time.h>

/** @fn main
 * @brief Main execution method.
 * 
 * Initialization of the minimax evaluation system.
 * 
 * @return int 
 */
int main(int argc, char** argv) 
{
    if (init_network(argc, argv) == -1)
    {
        return -1;
    }

    finished_main = false;
    pthread_mutex_init(&lock, NULL);

    if (node_id == '1')
    {
        responses_list = NULL;
        network_initialized = false;
        pthread_create(&network_thread_id, NULL, master_network_main, NULL);
    }

    else
    {
        request_buffer = NULL;
        response_buffer = NULL;
        worker_state = '0';
        request_len = 0;
        response_len = 0;
        pthread_create(&network_thread_id, NULL, worker_network_main, NULL);
    }

    node = node_id; // Node number

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

    // calc_level(conf->initial_state, 0);
    // if (node_id == '1')
    // {
    //     finished_main = true;
    // }
    if (node_id == '1')
    {
        pthread_create(&minimax_thread_id, NULL, master_minimax_main, NULL);
    }
    else
    {
        pthread_create(&minimax_thread_id, NULL, worker_minimax_main, NULL);
    }

    pthread_join(network_thread_id, NULL);
    pthread_join(minimax_thread_id, NULL);

    cpu_time_elapsed = clock() - cpu_time_elapsed;
    gettimeofday(&wall_stop, NULL);

    eval_thread_exec = false;
    clean_passant_temp();
    delete_configuration(conf);

    wall_seconds = (double)(wall_stop.tv_usec - wall_start.tv_usec) / 1000000
        + (double)(wall_stop.tv_sec - wall_start.tv_sec);

    pthread_join(eval_thread_id, NULL);

    if (node_id == '1')
    {
        export_report();
    }

    free_node_info();

    pthread_mutex_destroy(&lock);

    free_network();
    response* temp;
    while (responses_list != NULL)
    {
        temp = responses_list;
        responses_list = temp->next;
        free(temp);
    }
    
    return 0;
}