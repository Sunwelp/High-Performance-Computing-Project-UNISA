VF2++ implementation for graphs isomorphism check. The algorithm uses:

	1. C as main language;
	2. makefile logic to link files and compile the project;
	3. .sh script for testing and executing multiple runs.

/******************************************************************************************************************************/
The algorithm uses two text files, each with a graph inside, saved in the following format:

<number of nodes>\b<total file lines>
<source node 1>\t<neighbour_1>\b<neighbour_2>\b<neighbour_3>\b...\b<neighbour_N>\n
<source node 2>\t<neighbour_1>\b<neighbour_2>\b<neighbour_3>\b...\b<neighbour_N>\n
...\n
<source node N>\t<neighbour_1>\b<neighbour_2>\b<neighbour_3>\b...\b<neighbour_N>\n


/******************************************************************************************************************************/	
To run the algorithm, copy/paste the graphs in the specific format in the right folder, open a terminal console in
the project folder and run the following commands:
	a. make (compile the project and generate the launcher file)*;
	b. mpirun -n worker ./VF2pp_serial <tokenGraph.txt> <patternGraph.txt> <OPTIONAL:VERBOSE>;
	c. make clean (to remove the last compiled folder and launcher).

The argument used in the command line are:
	1. "tokenGraph.txt", is the token** graph file's name;
	2. "patternGraph.txt", is the pattern** graph file's name;
	3. "VERBOSE", is an optional*** argument used for printing useful information about the graph.

*   The "make" command used without argument, set by default the optimization to -O2. To modify the optimization use the command:
	make OPTIMIZATION="-Ox"
    and set "x" to the value of optimization needed, in the range [1,4].
  
**  The pattern/matching slang is used to differentiate between the main graph and the graphs to be tested:
    the Token Graph is the main graph, while the Pattern graph is the graph that is tested if it is isomorph
    to the Token Graph.
  
*** Standard value set to 0. If omitted, no information will be printed. If graph info are needed set this value to 1.
   Other values will be rejected and the program will continue as if the value was set to 0.
/******************************************************************************************************************************/
For multiple runs and testing, the bash script in the folder can be used. Simply write in the terminal:

./measures.sh <#ofRuns> <tokenGraph.txt> <patternGraph.txt> <OPTIONAL:OPTIMIZATION> <OPTIONAL:METRICS> <OPTIONAL:VERBOSE>

with the adequate number of arguments, which are:
	
	a. #ofRuns: define how many time the program will be executed;
	b. tokenGraph.txt", is the token graph file's name;
	c. "patternGraph.txt", is the pattern graph file's name;
	d. "OPTIMIZATION", set the optimization level for the make command (default is -O2);
	e. "METRICS", set the name for metrics CSV file;
	f. "VERBOSE", is an optional argument used for printing useful information about the graph.
	
Metrics and log info are saved in separated file: metrics go in the .CSV file though stdout, while logging info will be
save in a .log file though the stderr.

/************************************************** EXAMPLES ***************************************************************/
- Single run example commands:

    ./VF2pp_serial G_n10_c30.txt G_Iso_n10_c30.txt 0    	// Set VERBOSE to 0 -> do not print additional info
    ./VF2pp_serial G_n10_c30.txt G_Iso_n10_c30.txt 1    	// Set VERBOSE to 1 -> print additional info
    ./VF2pp_serial G_n10_c30.txt G_Iso_n10_c30.txt	    	// VERBOSE not defined (remain set to 0)
    ./VF2pp_serial G_n10_c30.txt G_Iso_n10_c30.txt foo		// invalid argument -> print a WARNING message and treat VERBOSE equal to 0

- Shell script example commands:

   ./measures.sh 2 G_n10_c30.txt G_Iso_n10_c30.txt -O1 results.csv 1   // Full command with 2 execution, set OPTIMIZATION, .CSV file name and VERBOSE mode
   ./measures.sh 10 G_n10_c30.txt G_Iso_n10_c30.txt 		       // Executes 10 run, with default optional values

   
    

