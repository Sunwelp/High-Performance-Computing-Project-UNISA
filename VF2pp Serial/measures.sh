' 
 Course: High Performance Computing 2023/2024
  
 Lecturer: Francesco Moscato	fmoscato@unisa.it
 
 Student:
 Pepe Lorenzo        0622702121      l.pepe29@studenti.unisa.it          
 
 
                REQUIREMENTS OF THE ASSIGNMENT:
 Design, Implement, and Analyze a parallel version of VF2-pp based on MPI
 
 
 Copyright (C) 2024 - All Rights Reserved
 
 This program is free software: you can redistribute it and/or modify it under the terms of 
 the GNU General Public License as published by the Free Software Foundation, either version 
 3 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 See the GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along with ContestOMP. 
 If not, see <http://www.gnu.org/licenses/>.
'
#!/bin/bash

if [ "$#" -lt 3 ]; then
    echo "Usage: $0 <#ofExecution> <Token_file name> <Pattern file name> [OPTIONAL: OPTIMIZATION] [OPTIONAL: METRICS_FILE][OPTIONAL: VERBOSE]"
    exit 1
fi

NUM_RUNS="$1"
TOKEN_FILE="$2"
PATTERN_FILE="$3"

OPTIMIZATION="${4:-}"
OPTIMIZATION="${OPTIMIZATION:-"-O2"}"

OUTPUT_FILE="${5:-}"
OUTPUT_FILE="${OUTPUT_FILE:-"metrics.csv"}"

VERBOSE="${6:-}"
VERBOSE="${VERBOSE:-"0"}"

if ! [[ "$NUM_RUNS" =~ ^[0-9]+$ ]] || [ "$NUM_RUNS" -lt 1 ] || [ "$NUM_RUNS" -gt 20 ]; then
    echo "ERROR: NUM_RUNS must be an integer between 1 and 20."
    exit 1
fi

if [[ "$OPTIMIZATION" != "-O0" && "$OPTIMIZATION" != "-O1" && "$OPTIMIZATION" != "-O2" && "$OPTIMIZATION" != "-O3" ]]; then
    echo "ERROR: OPTIMIZATION must be one of '-O0', '-O1', '-O2', or '-O3'."
    exit 1
fi

if [[ "$VERBOSE" != "0" && "$VERBOSE" != "1" ]]; then
    echo "ERROR: VERBOSE must be either '0' (no verbosity) or '1' (verbose)."
    exit 1
fi


make "OPTIMIZATION=$OPTIMIZATION" || { echo "ERROR: Make failed, exiting."; exit 1; }

LOG_FILE="info.log"
> "$LOG_FILE"
echo "Execution Info: Optimization=$OPTIMIZATION,Runs=$NUM_RUNS" > "$OUTPUT_FILE"
echo "Run,loadGraph,VF2pp" >> "$OUTPUT_FILE"


for((i = 1; i <= NUM_RUNS; i++))
do

	if [ "$VERBOSE" == "0" ]; then
		OUTPUT=$(./VF2pp_serial "$TOKEN_FILE" "$PATTERN_FILE" 2>> "$LOG_FILE")
	else
		OUTPUT=$(./VF2pp_serial "$TOKEN_FILE" "$PATTERN_FILE" "$VERBOSE" 2>> "$LOG_FILE")
	fi

	echo "$i,$OUTPUT" >> "$OUTPUT_FILE"

	echo "Run $i finished"
done

echo "All runs completed successfully. Results saved to $OUTPUT_FILE."
echo "Info log available in $LOG_FILE"

echo "Showing execution time results"
column -t -s, < "$OUTPUT_FILE"

echo "Cleaning make build"
make clean
exit 0