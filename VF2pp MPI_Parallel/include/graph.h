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
 
#ifndef VF2PP_GRAPH_H
#define VF2PP_GRAPH_H

#include "graph_components.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * @brief This function creates and initializes a graph with a defined number of nodes.
 *
 * @param num_nodes: number of nodes of the graph
 *
 * @return The created graph.
 */
Graph* createGraph(int num_nodes);

/**
 * @brief This function free the memory area assigned to the graph
 *
 * @param g: the graph structures to be deleted
 *
 */
void freeGraph(Graph* g);

/**
 * @brief This function create the neighbour list of a node
 *
 * @param g:          the graph structures
 * @param node:       node from where the edge start
 * @param list:       node's neighbour list
 * @param neighbours: total number of neighbours
 *
 */
void addEdge(Graph* g, int node, int* list, int neighbours);

/**
 *  @brief  Read graph from a text file and call functions to create
 *          the structure in memory
 *
 *  @param  filename:   path of the file that contains the graph
 *  @param  graphName:  name of the Graph variable
 *  @param  VERBOSE:    set the Verbose mode
 *
 *  @return the created graph
 */
Graph* readGraphFromFile(const char *filename, bool VERBOSE);

/**
 * @brief Get a line with a starting node and it's neighbour list
 *        and call "addEdge()" function to add the node and it's list
 *        to the graph
 *
 * @param line: array containing all the numbers in the line
 * 
 */
void process_line(Graph* g, char *line, int isFirstLine);
/**
 *  @brief Print a graph
 *
 *  @param g: the graph to print
 *
 */
void printGraph(Graph* g);

/************* FLAT GRAPH FUNCTION PROTOTYPES ************/

/**
 *  @brief	Flatten a graph, used to be able to send the graph through MPI  
 * 
 *  @param	g:	pointer to the graph to be flattened
 *  @param  flat_size: size of the array to be allocated
 * 
 */
int* flatten_graph(Graph* g, int* flat_size);

/**
 *  @brief  Unflatten a graph back into the original struct
 * 
 *  @param flat: the flattened graph
 * 
 *	@return The original graph 
 */
Graph* unflatten_graph(int* flat);

#endif //VF2PP_GRAPH_H
