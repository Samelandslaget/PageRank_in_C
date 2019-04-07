#include <stdio.h>
#include <stdlib.h>
#include "PE_functions_15220.h"
#include <omp.h>

int main(int argc, char const *argv[])
{
	// Make sure the necessary information is provided.
	if (argc<5)
	{
		printf("Usage: FILENAME DAMPING_CONSTANT CONVERGENCE_THRESHOLD N .\n");
		return 1;
	}

	// Store the CLI arguments into appropriate variables.
	// NOTE: The user is responsible for providing sane input.
	char const *filename = argv[1];
	double const d = atof(argv[2]);
	double const eps = atof(argv[3]);
	int n = atoi(argv[4]);

	// read_graph_from_file
	printf("Calling read_graph_from_file(%s)...\n",filename);
	double start_read = omp_get_wtime();
	struct CRS A = read_graph_from_file(filename);
	double end_read = omp_get_wtime();
    double tot_read = end_read - start_read;
    printf("Done!\n");
    printf("Time spent by read_graph_from_file: %lf s\n", tot_read);

	// PageRank_iterations
	int const superiterations = 15;
	printf("Calling PageRank_iterations(%.4lf, %.4lf, A) %d times...\n",d,eps,superiterations);
	double super_tot = 0, start_PageRank, end_PageRank, tot_PageRank;
	double *x;
	for (int i = 0; i < superiterations; ++i)
	{
		start_PageRank = omp_get_wtime();
		x = PageRank_iterations(d,eps,&A);
		end_PageRank = omp_get_wtime();
	    tot_PageRank = end_PageRank - start_PageRank;
	    super_tot += tot_PageRank;
		free(x);
	}
    printf("Done!\n");
    double average = super_tot/superiterations;
    printf("Average time spent by PageRank_iterations: %lf s\n", average);

    //Free stuff!
    free(A.row_ptr);
    free(A.col_idx);
    free(A.vals);
    free(A.dangling_idx);

    #pragma omp parallel
    {
    	#pragma omp single
    	{
    		printf("No. of threads: %d\n", omp_get_num_threads());
    	}
    }
	return 0;
}