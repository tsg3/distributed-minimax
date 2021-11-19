#include <network/network.h>

/**
 * Responses list.
 */

void add_response(int value)
{
    response* new_response = (response*)malloc(sizeof(response));
    new_response->result = value;
    new_response->next = NULL;

    if (responses_list == NULL)
    {
        responses_list = new_response;
    }

    else
    {
        response* temp = responses_list;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_response;
    }
}

/**
 * Initialization and free methods.
 */

int init_network(int argc, char** argv)
{
    FILE* fd;
    if ((fd = fopen("/etc/hostname", "r")) == NULL)
    {
        perror("Error opening the file!");
        return -1;
    }

    char hostname[7];
    fread(hostname, 1, 7, fd);
    fclose(fd);

    node_id = hostname[6];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    return 0;
}

void free_network()
{
    MPI_Finalize();
}

/**
 * Network
 * 
 * Master
 */

void* master_network_main()
{
    worker_status = 0;
    int flag = -1;

    char* buffer = NULL;
    int message_len;

    MPI_Status status;

    int i = 0;
    for (int j = 0; j < 4; j++)
    {
        if (j != process_rank)
        {
            worker_ranks[i] = j;
            i++;
        }
    }
    
    i = 1;
    int recv = 0;

    pthread_mutex_lock(&lock);
    network_initialized = true;
    pthread_mutex_unlock(&lock);

    while (1)
    {
        pthread_mutex_lock(&lock);
        if (finished_main)
        {
            pthread_mutex_unlock(&lock);
            break;
        }
        
        for (int j = 0; j < 3; j++)
        {
            if (BIT_CHECK(worker_status, j) == 1)
            {
                MPI_Iprobe(worker_ranks[j], 2, 
                    MPI_COMM_WORLD, &flag, &status);

                if (flag)
                {
                    MPI_Get_count(&status, MPI_BYTE, &message_len);
                    buffer = (char*)malloc(sizeof(char) * message_len);
                    MPI_Recv(buffer, message_len, MPI_BYTE, 
                        worker_ranks[j], 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                    BIT_CLEAR(worker_status, j);
                    printf("[ID: %c | Process: %d] | Got %s from process %d \n", 
                        node_id, process_rank, buffer, worker_ranks[j]);

                    /** Call function to format the message into it int value.
                     *  [This is a test] 
                     */
                    
                    int value;
                    sscanf(buffer, "%d", &value);
                    free(buffer);

                    /** ------------------------------------------------------ */

                    add_response(value);
                }
            }
        }
        pthread_mutex_unlock(&lock);

        usleep(500000);
    }

    State* state;
    int level;
    test(&state, &level);
    char* final_message = data_to_json(state, level);
    int final_message_len = strlen(final_message);

    // printf("[ID: %c | Process: %d] | Final message:\n%s\n", 
    //     node_id, process_rank, final_message);

    delete_state(state);

    MPI_Send(final_message, final_message_len, MPI_BYTE, 
        worker_ranks[0], 0, MPI_COMM_WORLD);
    MPI_Send(final_message, final_message_len, MPI_BYTE, 
        worker_ranks[1], 0, MPI_COMM_WORLD);
    MPI_Send(final_message, final_message_len, MPI_BYTE, 
        worker_ranks[2], 0, MPI_COMM_WORLD);
    
    free(final_message);

    printf("[ID: %c | Process: %d] | Finishing ... \n", node_id, process_rank);

    return NULL;
}

/**
 * Worker
 */

void* worker_network_main()
{
    int flag; 
    char* buffer = NULL;

    MPI_Status status;

    while (1)
    {
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

        if (flag) 
        {
            MPI_Get_count(&status, MPI_BYTE, &request_len);
            buffer = (char*)malloc(sizeof(char) * request_len);
            MPI_Recv(buffer, request_len, MPI_BYTE, status.MPI_SOURCE, status.MPI_TAG,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (status.MPI_TAG == 0)
            {
                pthread_mutex_lock(&lock);
                finished_main = true;
                pthread_mutex_unlock(&lock);

                // printf("[ID: %c | Process: %d] | Got this from %d:\n%s\n", 
                //     node_id, process_rank, status.MPI_SOURCE, buffer);
                
                State* state;
                int level;
                json_to_data(buffer, &state, &level); 
                // This is how it gets 
                //    the buffer data.

                delete_state(state);
                free(buffer);
                break;
            }
            
            else // status.MPI_TAG == 1
            {
                pthread_mutex_lock(&lock);

                request_buffer = (char*)malloc(sizeof(char) * request_len);
                memcpy(request_buffer, buffer, request_len);
                free(buffer);

                pthread_mutex_unlock(&lock);

                while (1)
                {
                    pthread_mutex_lock(&lock);
                    if (worker_state == '1')
                    {
                        worker_state = '0';

                        MPI_Send(response_buffer, response_len, MPI_BYTE, 
                            status.MPI_SOURCE, 2, MPI_COMM_WORLD);

                        free(response_buffer);
                        pthread_mutex_unlock(&lock);
                        break;
                    }
                    pthread_mutex_unlock(&lock);
                    usleep(500000);
                }
            }
        }
    }

    printf("[ID: %c | Process: %d] | Finishing ... \n", node_id, process_rank);

    return NULL;
}

/**
 * Minimax
 * 
 * Master 
 */

void* master_minimax_main()
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        if (network_initialized)
        {
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);
        usleep(500000);
    }

    int available;
    int sum = 0;
    for (int i = 1; i < 20; i++)
    {
        if ((available = worker_available(false)) != -1)
        {
            request_worker(i, available);
        }

        else
        {
            sum += i;
            printf("[ID: %c | Process: %d] | 'sum' + %d = %d \n", 
                node_id, process_rank, i, sum);
            sleep(1);
        }
    }

    while (1)
    {
        if (worker_available(true) == 1)
        {
            break;
        }
        usleep(500000);
    }

    response* temp;
    while (responses_list != NULL)
    {
        temp = responses_list;
        responses_list = temp->next;

        sum += temp->result;
        printf("[ID: %c | Process: %d] | 'sum' + %d = %d \n", 
            node_id, process_rank, temp->result, sum);

        free(temp);
    }

    pthread_mutex_lock(&lock);
    finished_main = true;
    pthread_mutex_unlock(&lock);

    return NULL;
}

/**
 * Worker
 */

void* worker_minimax_main()
{
    while (1)
    {
        pthread_mutex_lock(&lock);

        if (finished_main)
        {
            pthread_mutex_unlock(&lock);
            break;
        }

        if (request_buffer != NULL)
        {
            /** Parse message to int */
            int request;
            sscanf(request_buffer, "%d", &request);
            /** -------------------- */

            response_len = request_len; // Just because the final number is equal.
            free(request_buffer);
            request_buffer = NULL;
            worker_state = '2';

            pthread_mutex_unlock(&lock); 

            sleep(2); // 'Minimax'
            printf("[ID: %c | Process: %d] | Made the minimax for %d \n", 
                node_id, process_rank, request);

            pthread_mutex_lock(&lock);

            /** Parse message to char array */
            response_buffer = (char*)malloc(sizeof(char) * response_len);
            snprintf(response_buffer, response_len, "%d", request); // Here, 'request' should be the result.
            /** --------------------------- */
            worker_state = '1';
            pthread_mutex_unlock(&lock);
        }

        else
        {
            pthread_mutex_unlock(&lock);
            usleep(500000);
        }
    }

    return NULL;
}

/**
 * Check
 */

int worker_available(bool mode)
{
    int res = -1;
    pthread_mutex_lock(&lock);

    if (mode)
    {
        res = BIT_CHECK(worker_status, 0) == 0 &&
            BIT_CHECK(worker_status, 1) == 0 &&
            BIT_CHECK(worker_status, 2) == 0 ? 1 : 0;
    }

    else
    {
        if (BIT_CHECK(worker_status, 0) == 0)
        {
            res = 0;
        }

        else if (BIT_CHECK(worker_status, 1) == 0)
        {
            res = 1;
        }

        else if (BIT_CHECK(worker_status, 2) == 0)
        {
            res = 2;
        }
    }

    pthread_mutex_unlock(&lock);
    return res;
}

/**
 * Request
 */

void request_worker(int value, int pos)
{
    /** Call function to format the message into the char array buffer.
     *  [This is a test] 
     */
    int temp = value, buff_size = 1; // Actual size
    while (temp != 0)
    {
        temp /= 10;
        buff_size++;
    }
    char* buffer = (char*)malloc(sizeof(char) * buff_size);
    snprintf(buffer, buff_size, "%d", value);
    /** --------------------------------------------------------------- */

    pthread_mutex_lock(&lock);

    BIT_SET(worker_status, pos);
    MPI_Send(buffer, buff_size, MPI_BYTE, worker_ranks[pos], 1, MPI_COMM_WORLD);

    pthread_mutex_unlock(&lock);
    free(buffer);
}