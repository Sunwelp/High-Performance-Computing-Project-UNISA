VF2++ implementation for graphs isomorphism check. The algorithm uses:

	1. C as main language;
	2. makefile logic to link files and compile the project;
	3. .sh script for testing and executing multiple runs.

The algorithm uses two text files, each with a graph inside, saved in the following format:

number_of_nodes total_file_lines <br/>
source_node_1    neighbour_1 neighbour_2 neighbour_3 ... neighbour_N <br/>
source_node_2    neighbour_1 neighbour_2 neighbour_3 ... neighbour_N <br/>
... <br/>
source_node_N    neighbour_1 neighbour_2 neighbour_3 ... neighbour_N <br/>

To run the algorithm, copy/paste the graphs in the specific format in the right folder, open a terminal console in the project folder and run the following commands: <br/>
	a. make (compile the project and generate the launcher file)*; <br/>
	b. mpirun -n worker ./VF2pp_serial tokenGraph.txt patternGraph.txt OPTIONAL:VERBOSE; <br/>
	c. make clean (to remove the last compiled folder and launcher). <br/>

The argument used in the command line are: <br/>
	1. "tokenGraph.txt", is the token** graph file's name; <br/>
	2. "patternGraph.txt", is the pattern** graph file's name; <br/>
	3. "VERBOSE", is an optional*** argument used for printing useful information about the graph. <br/>

(*)   The "make" command used without argument, set by default the optimization to -O2. To modify the optimization use the command: 
	  make OPTIMIZATION="-Ox" and set "x" to the value of optimization needed, in the range [1,4]. <br/>
  
(**)  The pattern/matching slang is used to differentiate between the main graph and the graphs to be tested: the Token Graph is the main graph, while the Pattern 		  graph is the graph that is tested if it is isomorph to the Token Graph. <br/>
  
(***) Standard value set to 0. If omitted, no information will be printed. If graph info are needed set this value to 1. Other values will be rejected and the 			  program will continue as if the value was set to 0. <br/>
   
For multiple runs and testing, the bash script in the folder can be used. Simply write in the terminal: <br/>

./measures.sh #ofRuns tokenGraph.txt patternGraph.txt OPTIONAL:OPTIMIZATION OPTIONAL:METRICS OPTIONAL:VERBOSE <br/>

with the adequate number of arguments, which are: <br/>
	
	a. #ofRuns: define how many time the program will be executed;
	b. tokenGraph.txt", is the token graph file's name;
	c. "patternGraph.txt", is the pattern graph file's name;
	d. "OPTIMIZATION", set the optimization level for the make command (default is -O2);
	e. "METRICS", set the name for metrics CSV file;
	f. "VERBOSE", is an optional argument used for printing useful information about the graph.
	
Metrics and log info are saved in separated file: metrics go in the .CSV file though stdout, while logging info will be save in a .log file though the stderr.
