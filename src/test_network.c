#include <network/network.h>

int main(int argc, char** argv)
{
    if (init_network(argc, argv) == -1)
    {
        return -1;
    }

    responses_list = NULL;
    network_initialized = false;
    request_buffer = -1;
    response_buffer = -1;
    finished_main = false;
    worker_state = '0';
    pthread_mutex_init(&lock, NULL);

    if (node_id == '1')
    {
        pthread_create(&network_thread_id, NULL, master_network_main, NULL);
        pthread_create(&minimax_thread_id, NULL, master_minimax_main, NULL);
    }

    else
    {
        pthread_create(&network_thread_id, NULL, worker_network_main, NULL);
        pthread_create(&minimax_thread_id, NULL, worker_minimax_main, NULL);
    }

    pthread_join(network_thread_id, NULL);
    pthread_join(minimax_thread_id, NULL);

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