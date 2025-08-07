/* 
 * Course: High Performance Computing 2023/2024
 * 
 * Lecturer: Francesco Moscato	fmoscato@unisa.it
 *
 * Student:
 * Pepe Lorenzo        0622702121      l.pepe29@studenti.unisa.it          
 * 
 *
 *               REQUIREMENTS OF THE ASSIGNMENT:
 * Design, Implement, and Analyze a parallel version of VF2-pp based on MPI
 *
 *
 * Copyright (C) 2024 - All Rights Reserved
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation, either version 
 * 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with ContestOMP. 
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "graph.h"
#include "vf2pp.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

    clock_t start, end;
    double loadGraph, VF2pp;
/* Enable/Disable the verbose mode (printing more information during the run) */
    int VERBOSE = 0;

    if(argc < 3){
        fprintf(stderr, "ERROR: wrong number of arguments in command line.\n");
        fprintf(stderr, "Usage: %s <token_file_name.txt> <pattern_file_name.txt> <OPTIONAL:VERBOSE>\n", argv[0]);
        return EXIT_FAILURE;
    }else if(argc == 4){
        char *endptr;
        long int val = strtol(argv[3], &endptr, 10);
        if (argv[3] != endptr && *endptr == '\0') {
            if (val == 0 || val == 1) {
                VERBOSE = (int)val;
            }
        }else{
            fprintf(stderr, "WARNING: VERBOSE must be 0 or 1. Setting VERBOSE mode to 0.\n");
        }
    }
/******************** GRAPHS MANAGER LOGIC ********************/
    start = clock();
    /* Set folder path */
    const char* TokenFolder = "./Graphs/Token/";
    const char* PatternFolder = "./Graphs/Pattern/";

    /* Create the full path name */
    char TokenGraph[PATH_MAX];
    char PatternGraph[PATH_MAX];

    snprintf(TokenGraph, sizeof(TokenGraph), "%s%s", TokenFolder, argv[1]);
    snprintf(PatternGraph, sizeof(PatternGraph), "%s%s", PatternFolder, argv[2]);

    /* Create the graph */
    Graph* G1 = readGraphFromFile(TokenGraph, VERBOSE);
    if(G1 == NULL){
        fprintf(stderr, "ERROR: graph G1 not loaded.");
        return EXIT_FAILURE;
    }
    Graph* G2 = readGraphFromFile(PatternGraph, VERBOSE);
    if(G2 == NULL){
        fprintf(stderr, "ERROR: graph G2 not loaded.");
        return EXIT_FAILURE;
    }

    /* OPTIONAL: Print the graph's paths and their entire structure */
    if(VERBOSE){
        fprintf(stderr, "Token graph path: %s\n", TokenGraph);
        fprintf(stderr, "Pattern graph path: %s\n", PatternGraph);
        printGraph(G1);
        printGraph(G2);
    }
    end = clock();
    loadGraph = ((double)(end - start))/CLOCKS_PER_SEC;
/************************ VF2++ LOGIC ************************/
    start = clock();
    /* Initialize the isomorph check to false */
    bool isIso = false;

    /* Check results with VF2++ algorithm */
    isIso = vf2pp_is_isomorphic(G1, G2);
    
    VF2pp =  ((double)(end - start))/CLOCKS_PER_SEC;

    if(isIso){
        fprintf(stderr, "G1 and G2 are isomorph.\n");
    }else{
        fprintf(stderr, "G1 and G2 are not isomorph.\n");
    }
    /* Free the memory before exiting */
    freeGraph(G1);
    freeGraph(G2);

    end = clock();
    printf("%.4f,%.4f\n", loadGraph, VF2pp);
    return EXIT_SUCCESS;
}
