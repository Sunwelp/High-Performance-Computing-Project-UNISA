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
#include <mpi.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "graph.h"
#include "vf2pp.h"

#define MAX_FILENAME_LEN 256
#define FILENAMES_PATH "./Graphs/"
#define MAIN_GRAPH_PATH "./Graphs/Token/"
#define PATTERN_FOLDER "./Graphs/Pattern/"

int main(int argc, char *argv[]) {
/* Enable/Disable the verbose mode (printing more information during the run) */
    int VERBOSE = 0;

    if(argc < 2){
        fprintf(stderr, "ERROR: wrong number of arguments in command line.\n");
        fprintf(stderr, "Usage: mpirun -n <procs> %s <Graphs_file_name.txt> <OPTIONAL:VERBOSE>\n", argv[0]);
        return EXIT_FAILURE;
    }else if(argc == 3){
        char *endptr;
        long int val = strtol(argv[2], &endptr, 10);
        if (argv[2] != endptr && *endptr == '\0') {
            if (val != 0) {
                fprintf(stderr, "WARNING: this is a parallel program, VERBOSE mode is disabled. Setting VERBOSE to 0.\n");
            }
        }else{
            fprintf(stderr, "WARNING: VERBOSE must be an integer. Setting VERBOSE mode to 0.\n");
        }
    }
/*********************** MPI LOGIC ***********************/
    MPI_Init(&argc, &argv);
    int rank, size;

    double startTime = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /*********************** MASTER PROCESS LOGIC ***********************/
    if(rank == 0){
        /* Master rank read the main file, organize and send work to slave processes*/
        /* Step 1: gather all file name, remembering that list[0] is the main graph */
        char filename_path[PATH_MAX];
        if(snprintf(filename_path, sizeof(filename_path), "%s%s", FILENAMES_PATH, argv[1]) < 0){
            fprintf(stderr, "ERROR: full filenames path not created. Aborting program.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        FILE *list = fopen(filename_path, "r");
        if(!list){
            fprintf(stderr, "ERROR: file not opened. Aborting program.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        char filenames[size][MAX_FILENAME_LEN];
        int file_count = 0;
        for (; file_count < size && fscanf(list, "%255s", filenames[file_count]) == 1; file_count++){
            // Note: do nothing, all the work is done in the loop
        }
        if (file_count < size) {
            fprintf(stderr, "WARNING: Only %d graphs found (expected %d)\n", file_count, size);
        }
        fclose(list);

        /* Step 2: distribute filenames among all slave processes in a round robin policy */
        for(int i = 1; i < file_count; i++){
            int worker = (i -1)%(size - 1) + 1;     
            if(MPI_Send(filenames[i], MAX_FILENAME_LEN, MPI_CHAR, worker, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
                fprintf(stderr, "ERROR: Rank %d send failed for worker rank %d. Aborting program.\n", rank, worker);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }

        /* Step 3: load the main graph in memory */
        char main_graph_path[PATH_MAX];
        if(snprintf(main_graph_path, sizeof(main_graph_path), "%s%s", MAIN_GRAPH_PATH, filenames[0]) < 0){
            fprintf(stderr, "ERROR: full path for the main graph not created. Aborting program.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        Graph* main_graph = readGraphFromFile(main_graph_path, VERBOSE);
        if(!main_graph){
            fprintf(stderr, "ERROR: main graph not created. Aborting program.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        /* Step 4: flattening main graph */
        int flat_size = 0;
        int* flat_buffer = flatten_graph(main_graph, &flat_size);

        /* Step 5: broadcast size and flattened main graph to all the other processes */
        if(MPI_Bcast(&flat_size, 1, MPI_INT, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
            fprintf(stderr, "ERROR: Rank %d flat size broadcast failed. Aborting program.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if(MPI_Bcast(flat_buffer, flat_size, MPI_INT, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
            fprintf(stderr, "ERROR: Rank %d flat graph broadcast failed. Aborting program.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        /* Step 6: collect results */
        bool isIso[file_count-1];
        for(int i = 1; i < file_count; i++){
            int worker = (i -1)%(size - 1) + 1;
            if(MPI_Recv(&isIso[i - 1], 1, MPI_C_BOOL, worker, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS){
                fprintf(stderr, "ERROR: Rank 0 receive failed for worker rank %d. Aborting program.\n", worker);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }
        fprintf(stderr, "Results collected. Printing results:\n");
        for(int i = 0; i < file_count-1; i++){
            fprintf(stderr, "Main Graph and G%d are %s.\n", i+1, isIso[i] ? "isomorph" : "not isomorph");
        }
        /* Step 7: free the memory allocated for the main graph and the flat graph*/
        free(flat_buffer);
        freeGraph(main_graph);

        double endTime = MPI_Wtime();
        double cTime = endTime - startTime;
        printf("%d,%.4f", rank, cTime);

    }
    /********************** SLAVES PROCESSES LOGIC **********************/
    else{
        /* Slave processes receive work, load the pattern graphs and gives back results */
        /* Step 1: receive assigned file name and create the full path */
        char filename[MAX_FILENAME_LEN];
        char full_worker_path[PATH_MAX];
        if(MPI_Recv(filename, MAX_FILENAME_LEN, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS){
            fprintf(stderr, "ERROR: worker %d has failed to receive the filename of the assigned graph. Aborting program.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        if(snprintf(full_worker_path, sizeof(full_worker_path), "%s%s", PATTERN_FOLDER, filename) < 0){
            fprintf(stderr, "ERROR: worker %d has failed to recreate the full path for it's assigned graph. Aborting program.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        /* Step 2: load patten graph and launch VF2++ coimparison */
        bool worker_result = false;
        Graph* worker_graph = readGraphFromFile(full_worker_path, VERBOSE);
        if(!worker_graph){
            fprintf(stderr, "ERROR: worker %d has failed to load it's assigned graph. Aborting program.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        /* Step 3: receive flattened size and flattened main graph */
        int flat_size = 0;
        if (MPI_Bcast(&flat_size, 1, MPI_INT, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
            fprintf(stderr, "ERROR: worker %d has failed to receive flat main graph size. Aborting program.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int* received_flattened_graph = malloc(flat_size*sizeof(int));
        if(!received_flattened_graph){
            fprintf(stderr, "ERROR: worker %d has failed to allocate memory for receiving flat graph. Aborting program.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if(MPI_Bcast(received_flattened_graph, flat_size, MPI_INT, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
            fprintf(stderr, "ERROR: worker %d has failed to receive the broadcasted main graph. Aborting program.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        /* Step 4: unflat the main graph back into the original struct and free the memory assigned to the received flat graph */
        Graph* unflattened_main_graph = unflatten_graph(received_flattened_graph);

        /* Step 5: launch VF2++ coimparison */
        worker_result = vf2pp_is_isomorphic(unflattened_main_graph, worker_graph);

        /* Step 6: send result and free the memory from the worker graph */
        if(MPI_Send(&worker_result, 1, MPI_C_BOOL, 0, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
            fprintf(stderr, "ERROR: worker %d has failed to send it's result. Aborting program.\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        freeGraph(worker_graph);
        freeGraph(unflattened_main_graph);
    }

/********************** Finalize MPI logic and close the program ***********************/
    MPI_Finalize();
    return EXIT_SUCCESS;
}