#include <network/network.h>

int test(int argc, char** argv)
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
    
    int process_rank, cluster_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if ((process_rank & 0b1) == 0)
    {
        int number;
        for (int i = 2; i > 0; i--)
        {
            MPI_Send(&i, 1, MPI_INT, process_rank + 1, 1, MPI_COMM_WORLD);
            MPI_Recv(&number, 1, MPI_INT, process_rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        number = 0;
        MPI_Send(&number, 1, MPI_INT, process_rank + 1, 0, MPI_COMM_WORLD);

        printf("[%s | Process %d] | Finishing ... \n", hostname, process_rank);
    }

    else
    {
        int flag = -1; 
        int number;
        MPI_Request request;
        MPI_Status status;

        while (1)
        {
            if (flag == -1)
            {
                MPI_Irecv(&number, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
                flag = 0;
            }

            MPI_Test(&request, &flag, &status);

            if (flag) 
            { 
                if (status.MPI_TAG == 0)
                {
                    break;
                }
                
                else
                {
                    printf("[%s | Process %d] | P%d sent you this: %d\n", 
                        hostname, process_rank, status.MPI_SOURCE, number);

                    number = 0;
                    MPI_Send(&number, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
                }

                flag = -1;
            }
        }

        printf("[%s | Process %d] | Finishing ... \n", hostname, process_rank);
    }

    MPI_Finalize();
    return 0;
}