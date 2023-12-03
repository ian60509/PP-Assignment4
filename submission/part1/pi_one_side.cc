#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#pragma GCC optimize("Ofast", "unroll-loops")

int main(int argc, char **argv)
{   
    time_t cur_time =  time(NULL);
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---


    MPI_Win win;
    MPI_Comm_size( MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank( MPI_COMM_WORLD, &world_rank);
    long long int gather[world_size];

    
    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    

    // -------------- get the number of tosses in this processor ------------------
    long long int total_in_circle = 0;
    long long int private_num_in_circle = 0; //initial to 0
    long long int num_of_tosses = tosses/world_size;
    unsigned int seed = world_rank * cur_time;
    

    for ( int toss = 0; toss < num_of_tosses; toss ++) {
        double x =((double)rand_r(&seed) *2 / (RAND_MAX + 1.0)) -1 ;
        double y =((double)rand_r(&seed) *2 / (RAND_MAX + 1.0)) -1 ;
        
        double distance_squared = x * x + y * y;
        if ( distance_squared <= 1){ 
            private_num_in_circle++;
        }
    }

    //----------------------------------------
    long long int *result_in_master;
    if (world_rank == 0)
    {
        // Master
        MPI_Alloc_mem(sizeof(long long int), MPI_INFO_NULL, &result_in_master);
        *result_in_master = 0;
        MPI_Win_create(result_in_master, sizeof(long long int), sizeof(long long int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    }
    else
    {
        // Workers
        MPI_Win_create(NULL, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, 0, win); //gain lock
        MPI_Accumulate(&private_num_in_circle, 1, MPI_LONG_LONG_INT, 0, 0, 1, MPI_LONG_LONG_INT, MPI_SUM, win);
        MPI_Win_unlock(0, win);
    }

    MPI_Win_free(&win);

    if (world_rank == 0)
    {
        // TODO: handle PI result
        *result_in_master += private_num_in_circle;
        pi_result = 4.0 * (*result_in_master / (double)tosses);
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }
    
    MPI_Finalize();
    return 0;
}