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
#ifndef VF2PP_GRAPH_COMPONENTS_H
#define VF2PP_GRAPH_COMPONENTS_H

#include <stdbool.h>

/* Base graphs structures */
/**
 * @brief Struct that defines the Node component
 *
 * @param neighborhood:  list containing the neighbors of the node
 * @param num_neighbors: number of neighbors of the node
 * @param ID:            index of the node
 */
typedef struct{
    int* neighborhood;
    int num_neighbors;
    int ID;
    int mapped;
}Node;

/**
 * @brief Struct that defines the Graph
 *
 * @var num_nodes:  total number of nodes presents in the graph
 * @var nodes:      list containing all the nodes presents in the graph

 */
typedef struct{
    int num_nodes;
    Node* nodes;
}Graph;

#endif //VF2PP_GRAPH_COMPONENTS_H
