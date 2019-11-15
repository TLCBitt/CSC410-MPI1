#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Find out rank, size
    int myrank;
    int world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int list_size;
    if(argc <=2)
    {
         list_size = atoi(argv[1]);
    }
    int recv_size = list_size / world_size;

    int recv[recv_size];

    int arr[list_size];

    srand(0);
    if (myrank == 0)
    {
        printf("List: ");
        for (int i = 0; i < list_size; i++) {
            arr[i] = rand() % 10;
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    MPI_Scatter(arr, recv_size, MPI_INT, recv, recv_size, MPI_INT, 0, MPI_COMM_WORLD);
//Everyone find the sum of their part of the list
    int partial_sum = 0;
    for (int i = 0; i < recv_size; i++)
    {
        partial_sum += recv[i];
    }

    int partial_sums[world_size];
    MPI_Gather(&partial_sum, 1, MPI_INT, partial_sums, 1, MPI_INT,0, MPI_COMM_WORLD);

    MPI_Scatter(arr, recv_size, MPI_INT, recv, recv_size, MPI_INT, 0, MPI_COMM_WORLD);

//Everyone find the max of their part of the list
    int local_max = 0;
    for(int i= 0; i < recv_size; i++)
    {
        if(recv[i] > local_max)
            local_max = recv[i];
    }
    int local_maxs[world_size];
    MPI_Gather(&local_max, 1, MPI_INT, local_maxs, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(arr, recv_size, MPI_INT, recv, recv_size, MPI_INT, 0, MPI_COMM_WORLD);

//Everyone find the min of their part of the list
    int local_min = 10;
    for(int i = 0; i < recv_size; i++)
    {
        if(recv[i] < local_min)
            local_min = recv[i];
    }
    int local_mins[world_size];
    MPI_Gather(&local_min, 1, MPI_INT, local_mins, 1, MPI_INT, 0, MPI_COMM_WORLD);
 if ( myrank == 0 )
    {
        for ( int i = 0; i < world_size; ++i )
        {
            printf("Local Sum: %d \n", partial_sums[i]);
            printf("Local Max: %d\n", local_maxs[i]);
            printf("Local Min: %d\n", local_mins[i]);
            printf("\n");
        }
        printf("\n");
    }
 
    int red;
    MPI_Reduce( &partial_sum, &red, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if ( myrank == 0 )
        printf("Global Sum: %d\n", red);

    MPI_Reduce( &partial_sum, &red, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if ( myrank == 0 )
        printf("Global Max: %d\n", red);

    MPI_Reduce( &partial_sum, &red, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    if ( myrank == 0 )
        printf("Global Min: %d\n", red);


    MPI_Finalize();
    return 0;
}
