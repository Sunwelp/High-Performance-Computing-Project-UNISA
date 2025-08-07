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
/**
 *
 * @file node_ordering.c
 * @brief This file contains the functions useful for performing graph node sorting operations.
 * 
 * @dependencies
 * This module depends on the following files:
 * - node_ordering.h: This file defines data structures and functions useful for performing graph node sorting operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_ordering.h"

/**
 * This function compares two nodes based on their number of neighbors (degree).
 * It is used to sort nodes, giving priority to those with the highest degree. 
 * This sorting is important for strategies that favor nodes that are more “central” or “connected” in the graph.
 */
int compare_context(const void* a, const void* b) {
    Context* context_a = (Context*)a;
    Context* context_b = (Context*)b;
    return context_b->node->num_neighbors - context_a->node->num_neighbors;
}

/**
 * Initialize a breadth-first search (BFS) iterator. 
 * This iterator allows you to explore a graph level by level, starting from a specific node (start). 
 * It allocates the necessary memory and prepares the data to follow which nodes have been visited.
 */
BFSIterator* bfs_init(Graph* g, int start) {
    BFSIterator* iter = (BFSIterator*)malloc(sizeof(BFSIterator));
    iter->visited = (bool*)malloc(g->num_nodes * sizeof(bool));
    memset(iter->visited, 0, g->num_nodes * sizeof(bool));

    iter->current_level = (Level*)malloc(sizeof(Level));
    iter->next_level = (Level*)malloc(sizeof(Level));

    iter->current_level->capacity = 0;
    iter->next_level->capacity = 0;
    iter->current_level->nodes = NULL;
    iter->next_level->nodes = NULL;

    iter->current_level->size = 0;
    iter->next_level->size = 0;

    iter->visited[start] = 1;

    iter->current_level->capacity = 1;
    iter->current_level->nodes = (int*)malloc(iter->current_level->capacity * sizeof(int));
    iter->current_level->nodes[iter->current_level->size++] = start;

    return iter;
}

/**
 * Advances one level in the BFS iteration.
 * It examines the neighbors of nodes in the current layer, identifies nodes not yet visited, and adds them to the next layer. 
 * Returns the new layer to be explored, or NULL if the iteration is finished.
 */
Level* bfs_next(BFSIterator* iter, Graph* g) {
    if (iter->current_level->size == 0) {
        return NULL;  // If there are no more nodes to explore, it ends
    }

    iter->next_level->size = 0;

    // Calculates the sum of the neighbors of the nodes in the current layer
    int max_next_size = 0;
    for (int i = 0; i < iter->current_level->size; i++) {
        int node = iter->current_level->nodes[i];
        max_next_size += g->nodes[node].num_neighbors;
    }

    // Memory allocation for the next level
    iter->next_level->capacity = max_next_size;
    iter->next_level->nodes = (int*)malloc(iter->next_level->capacity * sizeof(int));

    // Explore the nodes of the current layer and update the next layer
    for (int i = 0; i < iter->current_level->size; i++) {
        int node = iter->current_level->nodes[i];
        for (int j = 0; j <g->nodes[node].num_neighbors; j++) {
            int nbr = g->nodes[node].neighborhood[j];
            if (!iter->visited[nbr]) {
                iter->visited[nbr] = true;
                iter->next_level->nodes[iter->next_level->size++] = nbr;
            }
        }
    }

    // Moves the next level to the current level for the next cycle
    Level* result = iter->current_level;
    iter->current_level = iter->next_level;
    iter->next_level = result;

    return iter->current_level;
}

/**
 * Frees the memory associated with a BFS iterator. 
 * This function ensures that all resources allocated during initialization and use of the iterator are properly released.
 */
void bfs_free(BFSIterator* iter) {
    free(iter->visited);
    free(iter->current_level->nodes);
    free(iter->next_level->nodes);
    free(iter->current_level);
    free(iter->next_level);
    free(iter);
}

/**
 * Determines an order for the nodes of a graph based on a combination of their degree and a BFS. Process:
 * 1. Finds the node with the highest degree and adds it to the order.
 * 2. It uses a BFS to explore its neighbors and orders them further by degree.
 * 3. Repeats the process until all nodes are sorted. 
 * The goal is to create an order that reflects some sort of “strategic priority” for applications such as optimization or graph matching.
 */
void matching_order(Graph* G1, int* node_order) {
    int size = G1->num_nodes;
    bool* ordered = (bool*)malloc(size*sizeof(bool));
    memset(ordered, 0, size * sizeof(bool));

    int num_nodes_ordered = 0;

    while (num_nodes_ordered < size) {
        // Find the node with the highest degree not yet sorted
        int max_neighbors = -1;  // Initialize the maximum to a low value
        int max_node = -1;  // Initializes the node index with the maximum number of neighbors

        for (int i = 0; i < G1->num_nodes; i++) {
          // If the node has more neighbors than the current maximum, update
          if (G1->nodes[i].num_neighbors > max_neighbors) {
              max_neighbors = G1->nodes[i].num_neighbors;
              max_node = i;  // Stores the index of the node
          }
        }
        node_order[num_nodes_ordered++] = max_node;
        ordered[max_node] = true;

        BFSIterator* iter = bfs_init(G1, max_node);
        Level* level;

        while ((level = bfs_next(iter, G1)) != NULL) {
            // Create a context array to sort nodes by neighbors
            Context* context_array = malloc(level->size * sizeof(Context));
            if (context_array == NULL) {
                perror("Insufficient memory");
                break;
            }

            for (int i = 0; i < level->size; i++) {
                int node_index = level->nodes[i];
                context_array[i].node_index = node_index;
                context_array[i].node = &G1->nodes[node_index];
            }

            // Sorting of Contexts by number of neighbors
            qsort(context_array, level->size, sizeof(Context), compare_context);

            for (int i = 0; i < level->size; i++) {
                node_order[num_nodes_ordered++] = context_array[i].node_index;
                ordered[context_array[i].node_index] = true;
            }

            free(context_array);
            context_array = NULL; 
        }

        bfs_free(iter);
        num_nodes_ordered++;

    }
    free(ordered);
}