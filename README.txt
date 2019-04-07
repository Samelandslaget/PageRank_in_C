Welcome to this README!

To compile the program:
type

`make pagerank.exe`

or

`gcc -o pagerank.exe PE_main_15220.c PE_functions_15220.c -fopenmp`

in the terminal.

To run the program:
type

`./pagerank. FILENAME DAMPING_CONSTANT CONVERGENCE_THRESHOLD N`

in the terminal.

Example run, assuming the web graph is contained in a file named 'web-NotreDame.txt':

./pagerank.exe web-NotreDame.txt 0.85 1e-15 10

Terminal output:
Calling read_graph_from_file(web-graphs/web-NotreDame.txt)...
Done!
Time spent by read_graph_from_file: 0.254185 s
Calling PageRank_iterations(0.8500, 0.0000, A)...
Done!
Time spent by PageRank_iterations: 0.698323 s
Calling top_n_webpages(10, x, 325729)...

Top 10 webpages:
Rank 0001 -- Score: 0.005626767, node No.:      1963
Rank 0002 -- Score: 0.005404512, node No.:         0
Rank 0003 -- Score: 0.003326229, node No.:    124802
Rank 0004 -- Score: 0.002857056, node No.:     12129
Rank 0005 -- Score: 0.002748731, node No.:    191267
Rank 0006 -- Score: 0.002731834, node No.:     32830
Rank 0007 -- Score: 0.002589985, node No.:      3451
Rank 0008 -- Score: 0.002459692, node No.:     83606
Rank 0009 -- Score: 0.002375901, node No.:      1973
Rank 0010 -- Score: 0.002340635, node No.:    142732
Done!
Time spent by top_n_webpages: 0.007734 s