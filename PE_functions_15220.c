#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "PE_functions_15220.h"

struct CRS read_graph_from_file(char const *Filename){
    FILE *fp; // Declare file pointer.
    char const *filename = Filename; // Name of our file.
    fp = fopen(filename, "r"); // Point file pointer to our file for reading.
    
    // Make sure no monkey business ensues.
    if (fp == NULL)
    {
        printf("Could not open file %s",filename);
        exit(0);
    }
    // This test does in fact cover very few potential bad files. If the text file provided by the user is not in the expected format, monkey business will indeed ensue.

    fscanf(fp, "%*[^\n]\n"); // Line 1 done.
    fscanf(fp, "%*[^\n]\n"); // Line 2 done.
    
    // Declare and correctly initialize nodes and edges.
    int nodes, edges;
    fscanf(fp, "%*c %*s %d %*s %d \n", &nodes, &edges); // Line 3 done (with some regex magic).
    fscanf(fp, "%*[^\n]\n"); // Line 4 done.
    
    // Store the links in arrays and count the number of self-links and outbound/inbound links from/to each node.
    int *fromNodeId = malloc(edges * sizeof *fromNodeId); // Will hold the first column of the web graph.
    int *toNodeId = malloc(edges * sizeof *toNodeId); // Will hold the second column of the web graph.
    int selflinks = 0;
    int *outbound = calloc(nodes, sizeof *outbound); // Effectively L[j].
    int *inbound = calloc(nodes, sizeof *inbound);
    int i;
    for (i = 0; i < edges; ++i)
    {
        fscanf(fp, "%d", &fromNodeId[i]);
        fscanf(fp, "%d", &toNodeId[i]);
        if (fromNodeId[i] == toNodeId[i])
            selflinks++;
        else
        {
            outbound[fromNodeId[i]]++;
            inbound[toNodeId[i]]++;
        }
    }

    // Close the file because we are not savage cavemen that do not care about our files after we are done with them. Additionally, our OS is probably happy to use the resources on something else.
    fclose(fp);

    // Dangling webpages:
    // #1 Count the number of dangling webpages.
    int dangling_count = 0;
    for (i = 0; i < nodes; ++i)
    {
        if (outbound[i] == 0)
        {
            dangling_count += 1;
        }
    }

    // #2 Allocate array to hold indices of dangling webpages.
    int *dangling_idx = malloc(dangling_count * sizeof *dangling_idx);

    // #3 Store the indices.
    int tmpIdx = 0;
    for (int i = 0; i < nodes; ++i)
    {
        if (outbound[i] == 0)
        {
            dangling_idx[tmpIdx] = i;
            tmpIdx++;
        }        
    }
    // Dangling webpages done.

    // Store the hyperlink matrix as CRS:
	// #1 Find the number of non-zero entries
	int nz = edges - selflinks;

	// #2 Allocate the appropriate arrays
	double *vals = malloc(nz * sizeof *vals);
	int *col_idx = malloc(nz * sizeof *col_idx);
	int *row_ptr = malloc((nodes+1)* sizeof *row_ptr);

    // #3 Initialize row_ptr
    row_ptr[0] = 0;
    for (i = 1; i < nodes+1; ++i)
    {
        row_ptr[i] = row_ptr[i-1] + inbound[i-1];
    }
    free(inbound); // Goodbye.

    // #4 Initialize col_idx and vals.
    int *tmp_row_ptr = malloc((nodes+1)* sizeof *tmp_row_ptr);
    memcpy(tmp_row_ptr, row_ptr, (nodes+1)* sizeof *tmp_row_ptr); // Make copy of row_ptr. Copy will be modified during initialization.
    for (int i = 0; i < edges; ++i)
    {
        if (fromNodeId[i] != toNodeId[i])
        {
	        col_idx[ tmp_row_ptr[toNodeId[i]] ] = fromNodeId[i]; // tmp_row_ptr points to the correct index in col_idx where the column index of the value will be stored.
	        vals [ tmp_row_ptr[toNodeId[i]] ] = 1.0/outbound[fromNodeId[i]]; // [Consider making outbound_inv] The value is then stored in the same index as we used in col_idx.
	        tmp_row_ptr[toNodeId[i]]++; //The next time we get to this row, we don't want to overwrite what we have previously done, so we must move one to the right. This does not risk overwriting the next row by moving too far to the right because of how row_ptr was created in the first place.
    	}
    }
    // This loop does not guarantee that the values in vals are in left-to-right order.
    // It does guarantee top-to-bottom order.
    // Because we always have the column index of the value, it should be fine.
    // Thus, this is technically not CRS, but something very similar.
    
    // Free stuff!
    free(tmp_row_ptr);
    free(outbound);

    // #5 Chuck all the data in a struct.
    struct CRS A;
    A.row_ptr = row_ptr;
    A.col_idx = col_idx;
    A.vals = vals;
    A.nodes = nodes;
    A.nz = nz;
    A.dangling_idx = dangling_idx;
    A.dangling_count = dangling_count;
    // Hyperlink matrix stored as CRS.

    // Return the struct containing the information needed to carry out the PageRank iterations.
    return A;
}

double* PageRank_iterations(double const d, double const eps, struct CRS *A)
{
    // Declare and initialize a bunch of stuff.
    int const N = A -> nodes; // N
    double *x = malloc(N*sizeof *x); // PageRank score vector.
    double *xp = malloc(N*sizeof *xp); // PageRank score vector from previous iteration.
    double *x_tmp; // For the pointer swap.
    double const Ninv = 1.0/N; // 1/N
    double const const1 = (1.0-d)*Ninv; // (1-d)/N
    double const const2 = d*Ninv; // d/N
    double W, first_term, second_term; // For computing x.
    int const dangling_count = A->dangling_count; // Because I can.
    double max_diff = eps + 0.1; // For checking convergence.
    int i, j; // Loop variables.
    
    // Fork.
    #pragma omp parallel
    {
        // Initialize xp.
        #pragma omp for
        for (i = 0; i < N; ++i)
        {
            xp[i] = Ninv;
        }
        
        // PageRank_iterations.
        // The implicit barrier at the bottom of the while-loop ensures no monkey business ensues.
        while(max_diff > eps)
        {
            // Compute x.
            // #1 Compute W.
            #pragma omp single
            {
                W = 0.0;
            }
            #pragma omp for reduction(+:W)
            for (i = 0; i < dangling_count; ++i)
            {
                W += xp[A->dangling_idx[i]];
            }

            // #2 Compute the first term of all the components in x.
            #pragma omp single
            {
                first_term = const1+const2*W;
            }

            // #3 Compute the second term and chuck both terms into x component-wise.
            #pragma omp for private(j, second_term)
            for (i = 0; i < N; ++i)
            {
                second_term = 0.0;
                for (j = A->row_ptr[i]; j < A->row_ptr[i+1]; ++j)
                {
                    second_term += A->vals[j]*xp[A->col_idx[j]];
                }
                x[i] = first_term + d*second_term;
            }
            // x computed.

            // Pointer swap and reset max_diff.
            #pragma omp single
            {
                x_tmp = x;
                x = xp;
                xp = x_tmp;
                max_diff = 0.0;
            }

            // Compute max_diff.
            #pragma omp parallel for reduction(max: max_diff)
            for (i = 0; i < N; ++i)
            {
                if (fabs(x[i]-xp[i])>max_diff)
                {
                    max_diff = fabs(x[i]-xp[i]);
                }
            }
        }
    }
    // Join.

    free(x); // Your job here is done.

    // x has converged. Because of pointer swap, xp is returned.
    return xp;
}

void top_n_webpages(int n, double *x, int N)
{
    // Declare what is needed.
    int top_counter = 0; // Keeps track of how many top webpages are found.
    double current_top_val = 0.0; // Keeps track of current top score.
    int current_top_idx; // Keeps track of the index of the webpage with the current top score.
    int i; // Loop variable.
    
    printf("\nTop %d webpages:\n", n);

    // Loop through the score vector x until the top n webpages are found.
    while(top_counter < n)
    {
        // Loop through x and find the top score with the corresponding page index.
        for (i = 0; i < N; ++i)
        {
            if (x[i]>current_top_val)
            {
                current_top_val = x[i];
                current_top_idx = i;
            }
        }

        // Increment top_counter. Print finds. Increment before print because of zero-based indexing.
        top_counter++;
        printf("Rank %.4d -- Score: %.9lf, node No.: %9d\n", top_counter, current_top_val, current_top_idx);

        // Reduce score of current top page to zero, so that we can find the page with the score just below the one we found this iteration.
        x[current_top_idx] = 0;

        // Reset current_top_val.
        current_top_val = 0.0;
    }     
}