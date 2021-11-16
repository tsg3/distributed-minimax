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

    int number;
    if ((process_rank & 0b1) == 0)
    {
        number = 666;
        MPI_Send(&number, 1, MPI_INT, process_rank + 1, 1, MPI_COMM_WORLD);
        printf("[%s | Process %d] | You sent this to P%d: %d\n", 
            hostname, process_rank, process_rank + 1, number);
    }

    else
    {
        MPI_Recv(&number, 1, MPI_INT, process_rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[%s | Process %d] | P%d sent you this: %d\n", 
            hostname, process_rank, process_rank - 1, number);
    }

    MPI_Finalize();
    return 0;
}