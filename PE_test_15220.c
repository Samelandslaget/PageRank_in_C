#include <stdio.h>
#include <stdlib.h>
#include "PE_functions_15220.h"
#include <math.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
	// // Make sure the necessary information is provided.
	// if (argc<5)
	// {
	// 	printf("Usage: FILENAME DAMPING_CONSTANT CONVERGENCE_THRESHOLD N .\n");
	// 	return 1;
	// }
	// // Store the CLI arguments into appropiate variables.
	// // NOTE: The user is responsible for providing sane input.
	// char const *filename = argv[1];
	// double d = atof(argv[2]);
	// double eps = atof(argv[3]);
	// int n = atoi(argv[4]);
	// printf("args: %lf, %lf, %d\n", d,eps,n); // DEBUG

	// // read_graph_from_file
	// double start_read = omp_get_wtime();
	// struct CRS hlmat = read_graph_from_file(filename);
	// double end_read = omp_get_wtime();
 //    double tot_read = end_read - start_read;
 //    printf("Time read_graph_from_file: %lf s\n", tot_read);
 //    printf("row_ptr\n");
 //    int *row_ptr_verify = malloc(9 * sizeof *row_ptr_verify);
 //    row_ptr_verify[0] = 0;
 //    row_ptr_verify[1] = 1;
 //    row_ptr_verify[2] = 4;
 //    row_ptr_verify[3] = 5;
 //    row_ptr_verify[4] = 6;
 //    row_ptr_verify[5] = 9;
 //    row_ptr_verify[6] = 12;
 //    row_ptr_verify[7] = 14;
 //    row_ptr_verify[8] = 17;
 //    for (int i = 0; i < hlmat.nodes+1; ++i)
 //    {
 //    	if (row_ptr_verify[i]-hlmat.row_ptr[i] != 0)
 //    	{
 //    		printf("No match!: %d %d \n", row_ptr_verify[i], hlmat.row_ptr[i]);
 //    	}
    	
 //    }
 //    double *col_sum = calloc(hlmat.nodes , sizeof *col_sum);
 //    for (int i = 0; i < hlmat.nz; ++i)
 //    {
 //    	col_sum[hlmat.col_idx[i]] += hlmat.vals[i];
 //    }
 //    int wrong_cols = 0;
 //    for (int i = 0; i < hlmat.nodes; ++i)
 //    {
 //    	if (fabs(col_sum[i] - 1.0) > eps)
 //    	{
 //    		if (col_sum[i] > eps)
 //    		{
	// 	    	printf("Col No. %.3d: %lf\n",i, col_sum[i]);
	// 	    	wrong_cols++;
 //    		}
 //    	}
 //    }
 //    printf("wrong_cols: %d\n", wrong_cols);
 //    printf("nzs: %d \n", hlmat.nz);
 //    for (int i = 0; i < hlmat.nz; ++i)
 //    {
 //    	printf("%.8lf at col No. %d\n", hlmat.vals[i], hlmat.col_idx[i]);
 //    }


	return 0;
}


// PE functions xxx.c. Proper comments and output
// info (using printf) should be provided. Time measurement commands
// around all the three functions should also be included.