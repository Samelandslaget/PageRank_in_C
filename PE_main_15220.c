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
	printf("Calling PageRank_iterations(%.4lf, %.4lf, A)...\n",d,eps);
	double start_PageRank = omp_get_wtime();
	double *x = PageRank_iterations(d,eps,&A);
	double end_PageRank = omp_get_wtime();
    double tot_PageRank = end_PageRank - start_PageRank;
    printf("Done!\n");
    printf("Time spent by PageRank_iterations: %lf s\n", tot_PageRank);

    //Free stuff!
    free(A.row_ptr);
    free(A.col_idx);
    free(A.vals);
    free(A.dangling_idx);

    // top_n_webpages
	printf("Calling top_n_webpages(%d, x, %d)...\n", n, A.nodes);
	double start_top_n = omp_get_wtime();
	top_n_webpages(n,x,A.nodes);
	double end_top_n = omp_get_wtime();
    double tot_top_n = end_top_n - start_top_n;
    printf("Done!\n");
	printf("Time spent by top_n_webpages: %lf s\n", tot_top_n);

	//More free stuff!
	free(x);

	return 0;
}