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
 */s
/**
 *
 * @file node_ordering.h
 * @brief This file defines data structures and functions useful for performing graph node sorting operations.
 * 
 * @dependencies
 * This module depends on the following files:
 * - graph.h: Definition of graph structure.
 */


#ifndef NODE_ORDERING_H
#define NODE_ORDERING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

/**
 * @struct Context
 * @brief This structure contains information of a node and its index within the graph.
 *
 * @var Context::node_index
 * Index of the node in the array `g->nodes`.
 * @var Context::node
 * Pointer to the node structure.
 */
typedef struct {
    int node_index; 
    Node* node;      
} Context;

/**
 * @struct Level
 * @brief This structure represents a layer of nodes in a BFS iteration.
 *
 * @var Level::nodes
 * Array of nodes in the layer.
 * @var Level::size
 * Number of nodes currently in the layer.
 * @var Level::capacity
 * Maximum level capacity.
 */
typedef struct {
    int* nodes;     
    int size;       
    int capacity;  
} Level;

/**
 * @struct BFSIterator
 * @brief This structure handles BFS iteration on the layers of a graph.
 *
 * @var BFSIterator::current_level
 * Pointer to current level.
 * @var BFSIterator::next_level
 * Pointer to the next level.
 * @var BFSIterator::visited
 * Boolean array to track visited nodes.
 */
typedef struct {
    Level* current_level;    
    Level* next_level;       
    bool* visited;            
} BFSIterator;


/**
 * @brief This function determines the order of the nodes in a graph 'G1'.
 *
 * @param G1 Pointer to graph.
 * @param node_order Array that will be populated with the order of the nodes.
 */
void matching_order(Graph* G1, int* node_order);

/**
 * @brief Comparison function used to sort an array of `Context` structures. Sorts the nodes in ascending order with respect to the number of neighbors.
 *
 * @param a Pointer to the first `Context` element.
 * @param b Pointer to the second `Context` element..
 * @return A negative value if the first node has fewer neighbors, zero if the number of neighbors is equal, a positive value if the first node has more neighbors.
 */
int compare_context(const void* a, const void* b);

/**
 * @brief This function creates a `BFSIterator` structure to start a Breadth-First Search (BFS) iteration from a specific node. The iterator manages the exploration levels and keeps track of the nodes visited.
 *
 * @param g Pointer to the graph to be explored. 
 * @param start Starting node for the BFS.
 * @return Pointer to a configured `BFSIterator` structure, or NULL if memory cannot be allocated.
 */
BFSIterator* bfs_init(Graph* g, int start);

/**
 * @brief This function calculates the next level of nodes during a BFS iteration.
 *
 * @param iter Pointer to an initialized `BFSIterator` structure.
 * @param g Pointer to the graph to be explored. 
 * @return Pointer to a `Level` structure representing the next level, or NULL if there are no more levels to explore.
 */
Level* bfs_next(BFSIterator* iter, Graph* g);

/**
 * @brief Frees all resources allocated by a BFS iterator.
 *
 * @param iter Pointer to the BFS iterator to be freed.
 */
void bfs_free(BFSIterator* iter);

#endif //NODE_ORDERING_H
