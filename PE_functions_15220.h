#ifndef PE_FUNCTIONS_INCLUDED
#define PE_FUNCTIONS_INCLUDED

struct CRS
{
	int *row_ptr;
	int *col_idx;
	double *vals;
	int nodes;
	int nz;
	
	int *dangling_idx;
	int dangling_count;
};
// This struct contains the necessary information to create a square matrix with side length nodes.
// The struct uses the CRS (Compressed Row Storage) format. The Wikipedia page explains this in detail.
// CRS is well suited for sparse matrices.

// Also stored in the struct is information about the columns whose elements are only zeros:
// dangling_idx holds the indices of these columns.
// dangling_count is how many such columns there are.


struct CRS read_graph_from_file(char const *Filename);
// Stores a web graph in the Compressed Row Storage format. Also stores information about dangling webpages.

// Returns a CRS struct -- Defined above.

// char const *Filename -- Holds the name of the text file containing the web graph.


double* PageRank_iterations(double const d, double const eps, struct CRS *A);
// Implements the iterative procedure of the PageRank algorithm. Please refer to the Partial Exam for details.

// Returns a converged PageRank score vector of type double*.

// double const d -- Damping constant.
// double const eps -- Convergence threshold.
// struct CRS *A -- Pointer to the hyperlink matrix stored as a CRS struct.


void top_n_webpages(int n, double *x, int nodes);
// Prints the top n webpages of a converged PageRank score vector along with their scores in the terminal.

// Returns nothing.

// int n -- The n in top n.
// double *x -- Converged PageRank score vector.
// int nodes -- Length of the score vector.
#endif