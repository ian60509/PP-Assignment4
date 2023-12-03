#include <mpi.h>
#include<stdio.h>
#include<stdlib.h>
#pragma GCC optimize("O3", "Ofast", "fast-math")
void show_matrix(const int *n_ptr, const int *m_ptr, const int *l_ptr, const int *a_mat_ptr, const int *b_mat_ptr, const int *c_mat_ptr, char which, int processor_rank){
    int n = *n_ptr;
    int m = *m_ptr;
    int l = *l_ptr;
    int a_size = n*m;
    int b_size = m*l;
    // printf("\n\n I'm %d Processor", processor_rank);
    if(which == 'A'){
        printf(" ------------ MATRIX \"A\" -----------------\n");
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                printf("%d ", a_mat_ptr[i*m+j]);
            }
            printf("\n");
        }
        printf("---------------------\n");
    }
    else if(which == 'B'){
        printf(" ------------ MATRIX \"B\" -----------------\n");
        for(int i=0; i<m; i++){
            for(int j=0; j<l; j++){
                printf("%d ", b_mat_ptr[i*l+j]);
            }
            printf("\n");
        }
        printf("---------------------\n");
    }
    else if(which == 'C'){
        // printf(" ------------ MATRIX \"C\" -----------------\n");
        for(int i=0; i<n; i++){
            for(int j=0; j<l; j++){
                printf("%d ", c_mat_ptr[i*l+j]);
            }
            printf("\n");
        }
        printf("---------------------\n");
    }
}
// Read size of matrix_a and matrix_b (n, m, l) and whole data of matrixes from stdin
//
// n_ptr:     pointer to n
// m_ptr:     pointer to m
// l_ptr:     pointer to l
// a_mat_ptr: pointer to matrix a (a should be a continuous memory space for placing n * m elements of int)
// b_mat_ptr: pointer to matrix b (b should be a continuous memory space for placing m * l elements of int)
void construct_matrices(int *n_ptr, int *m_ptr, int *l_ptr, int **a_mat_ptr, int **b_mat_ptr){
    int my_rank, world_size;
    int a_size=0;
    int b_size=0;
    MPI_Comm_size( MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank( MPI_COMM_WORLD, &my_rank);
    

    if(my_rank == 0){
        scanf("%d%d%d", n_ptr, m_ptr, l_ptr);
    }
    MPI_Bcast(n_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(m_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(l_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
    a_size = (*n_ptr) * (*m_ptr);
    b_size = (*m_ptr) * (*l_ptr);
    // malloc matrix storage space in each processor
    // use only 1D

    int*a = (int*) malloc(sizeof(int)*(a_size));
    memset(a, 0, a_size);
    int*b = (int*) malloc(sizeof(int)*(b_size));
    memset(b, 0, b_size);
    *a_mat_ptr = a;
    *b_mat_ptr = b;
    

    if(my_rank == 0){ //master processor read in 
        //get input of A
        for(int i=0; i<a_size; i++){
                scanf("%d", &a[i]);
        }
        // get input of B
        for(int i=0; i<b_size; i++){
                scanf("%d", &b[i]);
        }
    }
    
    // broadcast the matrix to all the other processor
    MPI_Bcast(*a_mat_ptr, a_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(*b_mat_ptr, b_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    // printf("I'm the %d-th node, n=%d\n",my_rank, *n_ptr);
    // if(my_rank==1) show_matrix(n_ptr, m_ptr, l_ptr, *a_mat_ptr, *b_mat_ptr, 'B', my_rank);

}

// Just matrix multiplication (your should output the result in this function)
// 
// n:     row number of matrix a
// m:     col number of matrix a / row number of matrix b
// l:     col number of matrix b
// a_mat: a continuous memory placing n * m elements of int
// b_mat: a continuous memory placing m * l elements of int
void matrix_multiply(const int n, const int m, const int l,const int *a_mat, const int *b_mat){

    int my_rank, world_size;
    MPI_Comm_size( MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank( MPI_COMM_WORLD, &my_rank);
    int a_size=n*m;
    int b_size=m*l;
    int c_size = n*l;
    int *private_C = (int*) calloc(c_size, sizeof(int));
    int *global_C = (int*) calloc(c_size, sizeof(int));

    int part_size = n/world_size;
    int my_start_row = my_rank * part_size;
    int my_end_row =(my_rank == world_size-1)? n-1: ((my_rank+1) * part_size) -1;

    for(int i=my_start_row; i<=my_end_row; i++){
        for(int j=0; j<l; j++){
            for(int k=0; k<m; k++){
                private_C[i*l+j] += a_mat[i*m+k] * b_mat[k*l+j];
            }
        }
    }

    // show_matrix(&n, &m, &l, a_mat, b_mat, private_C, 'C', my_rank);

    MPI_Reduce( private_C , global_C , c_size , MPI_INT , MPI_SUM , 0 , MPI_COMM_WORLD);
    if(my_rank==0){
        show_matrix(&n, &m, &l, a_mat, b_mat, global_C, 'C', my_rank);
    }

    free(private_C);
    free(global_C);
    return ;
}

// Remember to release your allocated memory
void destruct_matrices(int *a_mat, int *b_mat){
    free(a_mat);
    free(b_mat);
    return;
}