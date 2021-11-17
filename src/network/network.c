#include <network/network.h>

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

void* master_network_main()
{
    int buffer;
    worker_status = 0;
    int flag = -1;

    MPI_Status status_structs[3];

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
                MPI_Test(&request_structs[j], &flag, &status_structs[j]);

                if (flag)
                {
                    BIT_CLEAR(worker_status, j);
                    printf("[ID: %c | Process: %d] | Got %d from process %d \n", 
                        node_id, process_rank, recv_buffers[j], worker_ranks[j]);
                    add_response(recv_buffers[j]);
                }
            }
        }
        pthread_mutex_unlock(&lock);

        usleep(500000);
    }

    buffer = 0;
    MPI_Send(&buffer, 1, MPI_INT, worker_ranks[0], 0, MPI_COMM_WORLD);
    MPI_Send(&buffer, 1, MPI_INT, worker_ranks[1], 0, MPI_COMM_WORLD);
    MPI_Send(&buffer, 1, MPI_INT, worker_ranks[2], 0, MPI_COMM_WORLD);

    printf("[ID: %c | Process: %d] | Finishing ... \n", node_id, process_rank);

    return NULL;
}

void* worker_network_main()
{
    int flag = -1; 
    int buffer;

    MPI_Request request;
    MPI_Status status;

    while (1)
    {
        if (flag == -1)
        {
            MPI_Irecv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            flag = 0;
        }

        MPI_Test(&request, &flag, &status);

        if (flag) 
        { 
            if (status.MPI_TAG == 0)
            {
                pthread_mutex_lock(&lock);
                finished_main = true;
                pthread_mutex_unlock(&lock);
                break;
            }
            
            else // status.MPI_TAG == 1
            {
                pthread_mutex_lock(&lock);
                request_buffer = buffer;
                pthread_mutex_unlock(&lock);

                while (1)
                {
                    pthread_mutex_lock(&lock);
                    if (worker_state == '1')
                    {
                        worker_state = '0';
                        buffer = response_buffer;
                        response_buffer = -1;
                        pthread_mutex_unlock(&lock);

                        MPI_Send(&buffer, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
                        break;
                    }
                    pthread_mutex_unlock(&lock);
                    usleep(500000);
                }
            }

            flag = -1;
        }
    }

    printf("[ID: %c | Process: %d] | Finishing ... \n", node_id, process_rank);

    return NULL;
}

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

        if (request_buffer != -1)
        {
            int request = request_buffer;
            request_buffer = -1;
            worker_state = '2';
            pthread_mutex_unlock(&lock); 

            sleep(2); // 'Minimax'
            printf("[ID: %c | Process: %d] | Made the minimax for %d \n", 
                node_id, process_rank, request);

            pthread_mutex_lock(&lock);
            response_buffer = request; // Here, 'request' should be the result.
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

void request_worker(int value, int pos)
{
    pthread_mutex_lock(&lock);

    BIT_SET(worker_status, pos);
    MPI_Send(&value, 1, MPI_INT, worker_ranks[pos], 1, MPI_COMM_WORLD);
    MPI_Irecv(&recv_buffers[pos], 1, MPI_INT, worker_ranks[pos], 2, 
        MPI_COMM_WORLD, &request_structs[pos]);

    pthread_mutex_unlock(&lock);
}