VF2++ parallel implementation for graphs isomorphism check. The algorithm uses:

	1. C as main language;
	2. MPI as parallelization techonolgy;
	2. makefile logic to link files and compile the project.

/*******************************************************************************************************************************************************/
The algorithm uses several text files, each with a graph inside, saved in the following format:

<number of nodes>\b<total file lines>
<source node 1>\t<neighbour_1>\b<neighbour_2>\b<neighbour_3>\b...\b<neighbour_N>\n
<source node 2>\t<neighbour_1>\b<neighbour_2>\b<neighbour_3>\b...\b<neighbour_N>\n
...\n
<source node N>\t<neighbour_1>\b<neighbour_2>\b<neighbour_3>\b...\b<neighbour_N>\n

Furthermore, the algorithm needs a secondary file, containing all the graph file's names, formatted as follow:

<Token_graph_name.txt>\n
<Pattern1_graph_name.txt>\n
<Pattern2_graph_name.txt>\n
<Pattern3_graph_name.txt>\n
...\n
<Pattern-n_graph_name.txt>\n

N.B.: max number of files for the specific processor is 8 to achieve full parallel potential.


/*******************************************************************************************************************************************************/	
To run the algorithm, copy/paste the graphs in the specific format in the right folder, open a terminal console in
the project folder and run the following commands:
	a. make	(compile the project and generate the launcher file)*;
	b. mpirun -n workers ./VF2pp_parallel <Graphs.txt> <OPTIONAL:VERBOSE>;
	c. make clean (to remove the last compiled folder and launcher).

The argument used in the command line are:
	1. "workers", define the number of MPI workers (int in the range [1,8]);
	2. "Graph.txt", is the name of the file containing all the graph file's name;
	3. "VERBOSE", is an optional** argument used for printing useful information about the graph.
	
* The "make" command used without argument, set by default the optimization to -O2. To modify the optimization use the command:
	make OPTIMIZATION="-Ox"
  and set "x" to the value of optimization needed, in the range [1,4].

** Standard value set to 0. If omitted, no information will be printed. If graph info are needed set this value to 1.
  Other values will be rejected and the program will continue as if the value was set to 0.
  
/*******************************************************************************************************************************************************/
Examples (after launching "make"):

mpirun -n 4 ./VF2pp_parallel Filenames.txt 0	// 4 MPI worker, VERBOSE set to 0 -> do not print additional info
mpirun -n 8 ./VF2pp_parallel Filenames.txt 1	// 8 MPI worker, VERBOSE set to 1 -> Suppressed to 0: do not print additional info
mpirun -n 2 ./VF2pp_parallel Filenames.txt	// 2 MPI worker, VERBOSE not defined (remain set to 0)
mpirun -n 8 ./VF2pp_parallel Filenames.txt foo  // 8 MPI worker, invalid VERBOSE argument -> print a WARNING message and treat VERBOSE as if set to 0

NOTE: if the number of graphs in the file exceed the number of workers set, the extra graphs will be ignored.
Similarly, if there are more workers than graphs, the extra worker will be dormant. 
