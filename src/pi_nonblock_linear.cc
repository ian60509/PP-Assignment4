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
    long long int total_tosses = atoi(argv[1]);
    long long int total_in_circle = 0;
    int world_rank, world_size;
    MPI_Comm_size( MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank( MPI_COMM_WORLD, &world_rank);
    
    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    

    // -------------- get the number of tosses in this processor ------------------
    long long int num_in_circle = 0; //initial to 0
    long long int num_of_tosses = total_tosses/world_size;
    int x_positive = 0;
    int y_positive = 0;
    unsigned int seed = world_rank * cur_time;
    

    for ( int toss = 0; toss < num_of_tosses; toss ++) {
        double x =((double)rand_r(&seed) *2 / (RAND_MAX + 1.0)) -1 ;
        double y =((double)rand_r(&seed) *2 / (RAND_MAX + 1.0)) -1 ;
        
        double distance_squared = x * x + y * y;
        if ( distance_squared <= 1){ 
            num_in_circle++;
        }
    }
    
    
    // ---------------------------------------------------------------
    

    if (world_rank > 0)
    {   
        int dest = 0;
        MPI_Send(&num_in_circle, 1, MPI_LONG_LONG_INT, dest, 0, MPI_COMM_WORLD);
        
    }else if (world_rank == 0) //master
    {
        MPI_Request requests[world_size];
        MPI_Status status[world_size];
        long long int receive_data[world_size]; 
        receive_data[0] = num_in_circle;

        for (int i = 1;i < world_size;i++)
            MPI_Irecv(&(receive_data[i]), 1, MPI_LONG_LONG_INT, i, 0, MPI_COMM_WORLD, &(requests[i - 1]));

        MPI_Waitall(world_size - 1, requests, status);

        for (int i = 0;i < world_size;i++) {
            total_in_circle += receive_data[i];
        }
    }

    if (world_rank == 0)
    {
        // TODO: process PI result
        pi_result = 4.0 * (total_in_circle / (double)total_tosses);
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}





