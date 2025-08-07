VF2++ parallel implementation for graphs isomorphism check. The algorithm uses:

	1. C as main language;
	2. MPI as parallelization techonolgy;
	2. makefile logic to link files and compile the project.

The algorithm uses several text files, each with a graph inside, saved in the following format: <br/>

number_of_nodes total_file_lines <br/>
source_node_1    neighbour_1 neighbour_2 neighbour_3 ... neighbour_N <br/>
source_node_2    neighbour_1 neighbour_2 neighbour_3 ... neighbour_N <br/>
... <br/>
source_node_N    neighbour_1 neighbour_2 neighbour_3 ... neighbour_N <br/>

Furthermore, the algorithm needs a secondary file, containing all the graph file's names, formatted as follow:<br/>

<Token_graph_name.txt> <br/>
<Pattern1_graph_name.txt><br/>
<Pattern2_graph_name.txt><br/>
<Pattern3_graph_name.txt><br/>
...<br/>
<Pattern-n_graph_name.txt><br/>

N.B.: max number of files for the specific processor is 8 to achieve full parallel potential.

To run the algorithm, copy/paste the graphs in the specific format in the right folder, open a terminal console in the project folder and run the following commands:

	a. make	(compile the project and generate the launcher file)*;
	b. mpirun -n workers ./VF2pp_parallel Graphs.txt OPTIONAL:VERBOSE;
	c. make clean (to remove the last compiled folder and launcher).

The argument used in the command line are:

	1. "workers", define the number of MPI workers (int in the range [1,8]);
	2. "Graph.txt", is the name of the file containing all the graph file's name;
	3. "VERBOSE", is an optional** argument used for printing useful information about the graph.
	
(*) The "make" command used without argument, set by default the optimization to -O2. To modify the optimization use the command: <br/>
  make OPTIMIZATION="-Ox" and set "x" to the value of optimization needed, in the range [1,4]. <br/>

(**) Standard value set to 0. If omitted, no information will be printed. If graph info are needed set this value to 1. Other values will be rejected and the program will continue as if the value was set to 0.<br/>

NOTE: if the number of graphs in the file exceed the number of workers set, the extra graphs will be ignored. Similarly, if there are more workers than graphs, the extra worker will be dormant. 
