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
 * @file vf2pp.c
 * @brief This file contains the main functions used to check whether two graphs are isomorphic.
 * 
 * @dependencies
 * This module depends on the following files:
 * - graph.h: Definition of graph structure.
 * - node_ordering.h: Definition of the data structures and functions useful for performing graph node sorting operations.
 * - find_candidates.h: Definition of the functions to find nodes of G2 that are candidates to be mapped to a node of G1.
 * - stack.h: Definition of the data structures and functions to manage the stack that stores a node's candidates in order to keep track of mappings.
 * - vf2pp.h: Definition of the main functions used to check whether two graphs are isomorphic.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "node_ordering.h"
#include "find_candidates.h"
#include "stack.h"
#include "vf2pp.h"

/**
 * Restores the state of the Boolean vector T2_tilde when a previously mapped node is removed from the match.
 * This function handles the neighbors of the removed node (popped_node2) and updates T2_tilde to correctly reflect the availability of neighbor nodes for further matching.
 *  1. Iterates over all neighbors of the popped_node2 node in the G2 graph.
 *  2. For each neighbor:
 *      If the neighbor has an active match (mapping != -1), set an is_added flag to true.
 *      Otherwise, check whether the neighbors of the neighbor have a match. If none of these neighbors are mapped, mark the neighbor node as unavailable in T2_tilde.
 *  3. If none of the neighbors are mapped, re-add popped_node2 to T2_tilde, making it available for subsequent matches.
 */
void _restore_Tinout(Graph* G2, int popped_node2, bool* T2_tilde) {
    bool is_added = false; bool exit = false;
    for (int i = 0; i < G2->nodes[popped_node2].num_neighbors; i++) {
        int nbr = G2->nodes[popped_node2].neighborhood[i];
        if(G2->nodes[nbr].mapped != -1){
            is_added = true;
        }
        else {
            for (int j = 0; j <G2->nodes[nbr].num_neighbors; j++) {
                int nbr2 = G2->nodes[nbr].neighborhood[j];
                if ((G2->nodes[nbr2].mapped != -1)) {
                    exit = true;
                    break;
                }
            }
            if(exit){
                exit = false;
                continue;
            }
            else{
                T2_tilde[nbr] = false;
            }
        }
    }
    if(!is_added){
        T2_tilde[popped_node2] = true;
    }
}

/**
 * Update vector T2_tilde to reflect the new mapping of node new_node2. 
 * Once mapped, this node and its direct neighbors become unavailable for further matches. 
 * 1. Iterate on all neighbors of new_node2 in G2. 
 * 2. Set T2_tilde to false for each neighbor, making them unavailable. 
 * 3. Also mark new_node2 as unavailable by setting T2_tilde[new_node2] = false.
 */
void _update_Tinout(Graph* G2, int new_node2, bool* T2_tilde) {
    for(int i=0; i<G2->nodes[new_node2].num_neighbors; i++){
        int nbr = G2->nodes[new_node2].neighborhood[i];
        T2_tilde[nbr] = false;
    }
    T2_tilde[new_node2] = false;
}

/**
 * Implements the VF2++ algorithm to check whether two graphs G1 and G2 are isomorphic. 
 * Returns true if there is a biunique correspondence between the nodes of the two graphs preserving connections, otherwise false.
 * 
 * 1. Initial checks:
 *    Verify that both graphs have at least one node.
 *    Compare the number of nodes of G1 and G2. If different, the graphs cannot be isomorphic.
 *    Allocates and initializes T2_tilde (all true).
 *    Generates an optimal ordering of G1 nodes (node_order) for matching.
 * 
 * 2. Stack preparation:
 *    Initializes a stack to keep track of partial matches during the search.
 *    Finds the initial candidates for the first node in node_order and adds them to the stack.
 * 
 * 3. Match search:
 *    While the stack is not empty:
 *       Retrieves the current node (current_node) and its candidates.
 *       For each candidate not yet verified:
 *           Checks whether it is a valid mapping. If yes:
 *               Updates the data structures (mapping, T2_tilde) to reflect this mapping.
 *               Finds candidates for the next node and adds them to the stack.
 *           If all nodes are mapped, returns true (found a complete match).
 *       If no valid match is possible:
 *          Removes the current node from the stack and restores the previous state (_restore_Tinout).
 * 
 * 4. Final deallocation:
 *    Frees up memory used by T2_tilde, node_order, and the stack.
 *    Returns false if no complete match is found.
 */
bool vf2pp_is_isomorphic(Graph* G1, Graph* G2) {
    if (G1->num_nodes == 0 || G2->num_nodes == 0) {
        return false;
    }

    // Check basic graph properties
    if (G1->num_nodes != G2->num_nodes) {
        return false;
    }

    bool* T2_tilde =(bool*)malloc(G2->num_nodes*sizeof(bool));
    if(T2_tilde == NULL){
      fprintf(stderr, "ERROR: T2 Tilde error");
      return false;
    }
    memset(T2_tilde, 1, G2->num_nodes * sizeof(bool)); // Initialize all elements to true (1)

    int* node_order = (int*)malloc(G1->num_nodes*sizeof(int));
    if(node_order == NULL){
      fprintf(stderr, "ERROR: Node order error");
      return false;
    }
 
    matching_order(G1,node_order);// Get optimal node ordering
    
    // Main matching logic
    Stack stack;
    init_stack(&stack);

    int size_candidates;
    int* candidates = _find_candidates(node_order[0], G1, G2, &size_candidates, T2_tilde);

    NodeCandidates node;
    node.node = node_order[0];
    node.candidates = candidates;
    node.num_candidates = size_candidates;
    push(&stack, node);

    int matching_node = 1;
    int num_mapping = 0;

    while(stack.top != -1){
        bool found = false;
        NodeCandidates* node_candidate_current = &stack.elements[stack.top];
        int current_node = node_candidate_current->node;
        int candidates_nodes_size = node_candidate_current->num_candidates;
        for(int i=0; i< candidates_nodes_size; i++){
            int candidate = node_candidate_current->candidates[i];
            if(node_candidate_current->flags[i]== false){
                if (num_mapping == G2->num_nodes -1 ){
                    G1->nodes[current_node].mapped = candidate;
                    free_stack(&stack);
                    free(node_order);
                    free(T2_tilde);
                    return true;
                }
                node_candidate_current->flags[i] = true;
                G1->nodes[current_node].mapped = candidate;
                G2->nodes[candidate].mapped = current_node;
                num_mapping++;
                _update_Tinout(G2, candidate,T2_tilde);

                int size_next_candidates;
                int* next_candidates = _find_candidates(node_order[matching_node], G1, G2, &size_next_candidates,T2_tilde);
                NodeCandidates node_next;
                node_next.num_candidates = size_next_candidates;
                node_next.node = node_order[matching_node];
                node_next.candidates = next_candidates;
                push(&stack, node_next);

                matching_node++;
                found = true;
                break;
            }
        }
        if(found == false){
            pop(&stack);
            matching_node -= 1;
            if(stack.top != -1){
                NodeCandidates* popped_node = &stack.elements[stack.top];
                int popped_node1 = popped_node->node;
                int popped_node2 = G1->nodes[popped_node1].mapped;
                G1->nodes[popped_node1].mapped = -1;
                G2->nodes[popped_node2].mapped = -1;
                num_mapping -= 1;
                _restore_Tinout(G2, popped_node2, T2_tilde);
            }
        }
        continue;
    }
    free_stack(&stack);
    free(node_order);
    free(T2_tilde);
    return false;
}