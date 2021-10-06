#include <network/network.h>

int test(int argc, char** argv)
{
    int process_rank, cluster_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    int number;
    if (process_rank == 0)
    {
        number = 69;
        MPI_Send(&number, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        printf("Process %d | You sent this to P1: %d\n", process_rank, number);
    }

    else
    {
        MPI_Recv(&number, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d | P0 sent you this: %d\n", process_rank, number);
    }

    MPI_Finalize();
    return 0;
}