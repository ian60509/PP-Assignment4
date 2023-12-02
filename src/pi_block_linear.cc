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
    
    // printf("I'm %d-th processor: number of tosses of mine=%d\n",world_rank, num_of_tosses);

    for ( int toss = 0; toss < num_of_tosses; toss ++) {
        double x =((double)rand_r(&seed) *2 / (RAND_MAX + 1.0)) -1 ;
        double y =((double)rand_r(&seed) *2 / (RAND_MAX + 1.0)) -1 ;
        
        double distance_squared = x * x + y * y;
        // printf("x=%f, y=%f  distance_squared=%f\n",x,y,distance_squared );
        if ( distance_squared <= 1){ 
            num_in_circle++;
        }
    }
    // printf("x_positive++=%d, y_positive++=%d\n",x_positive++,y_positive++);
    // printf("num_in_circle = %lld\n",num_in_circle);
    // printf("num in circle = %d\n",num_in_circle);
    
    // ---------------------------------------------------------------
    

    if (world_rank > 0)
    {   
        int dest = 0;
        MPI_Send(&num_in_circle, 1, MPI_LONG_LONG_INT, dest, 0, MPI_COMM_WORLD);
        
    }else if (world_rank == 0) //master
    {
        total_in_circle = num_in_circle; // thread 0 自己的n_in_circle
        for (int i=1; i<world_size; i++){
            long long int private_num_in_circle = 0;
            MPI_Recv(&private_num_in_circle, 1, MPI_LONG_LONG_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_in_circle += private_num_in_circle;
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





