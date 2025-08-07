Python script to generate graphs:
		1. generate a graph with an user defined number of nodes and coverage (number of edge, in percentage);
		2. generate a specified number of secondary graphs, isomorphic to the first one, with same number of nodes and coverage.
		
To launch the script, open a terminal windows in the script folder and type:
	
		python3 main.py --nodes N --coverage C --isographs X

where:  	N = number of nodes (value must be > 0; default set to 10);
		C = coverage (accepted values in the range [10,70]; default set to 30);
		X = number of isomorphic graphs to be generated [accepted values in the range [1,8]; default set to 1).
	
Graphs are saved in:
		a. main graph in ./graphs/Token
		b. isomorphic graph in ./graphs/Pattern
