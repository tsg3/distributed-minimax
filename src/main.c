#include <stdio.h>
#include <configuration.h>
#include <evaluation/evaluation.h>

int main () 
{
    init_chess_models();
    conf = configure_minimax();

    if (conf == NULL)
    {
        clean_passant_temp();
        return -1;
    }

    pthread_create(&eval_thread_id, NULL, eval_main, NULL);

    print_configuration(conf);
    printf("%d\n", calc_level(conf->initial_state, 0));
    eval_thread_exec = false;

    pthread_join(eval_thread_id, NULL);

    printf("Total time -> %.2f seconds\n", (double) time_elapsed);

    Measure* temp = CPU_list;
    int i = 0;
    while (temp != NULL)
    {
        printf("CPU measure #%d at [%.2f seconds] -> CPU : %f\n", i, temp->time, temp->value);
        temp = temp->next;
        i++;
    }

    temp = RAM_list;
    i = 0;
    while (temp != NULL)
    {
        printf("RAM measure #%d at [%.2f seconds] -> RAM : %f\n", i, temp->time, temp->value);
        temp = temp->next;
        i++;
    }

    free_lists();

    clean_passant_temp();
    delete_configuration(conf);
    
    return 0;
}