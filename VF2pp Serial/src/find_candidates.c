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
 * @file find_candidates.c
 * @brief This file contains functions to find nodes of G2 that are candidates to be mapped to a node of G1.
 * 
 * @dependencies
 * This module depends on the following files:
 * - find_candidates.h: Defines functions to find nodes of G2 that are candidates to be mapped to a node of G1.
 * - graph.h: Definition of graph structure.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "find_candidates.h"

/**
 * This function finds candidate nodes in a G2 graph that could match a specific node u in another G1 graph.
 * The goal is to support matching algorithms between graphs.
 * 
 * 1. Identification of mapped neighbors:
 *    Searches the neighbors of u in G1 for those already mapped to nodes in G2.
 *    It collects them in a temporary covered_neighbords array.
 * 
 * 2. Case 1: No neighbors mapped:
 *    If none of the neighbors are mapped, considers all nodes in G2 that are not already mapped, are in T2_tilde, and have the same degree of u.
 *    
 * 3. Case 2: Only one neighbor mapped:
 *    Check the neighbors of the mapped node in G2 and select those that are not already mapped and have the same degree of u.
 * 
 * 4. Case 3: Multiple neighbors mapped:
 *    It uses an algorithm to find nodes in G2 that are common among the neighbors of all mapped nodes. 
 *    It filters these nodes based on the criteria of mapping and degree. 
 *    Sort the neighbors of each mapped node using qsort to optimize the comparison.
 * 
 * 5. Memory release:
 *    Frees up temporary memory (covered_neighbords) to prevent leakage.
 */
int* _find_candidates(int u, Graph* G1, Graph* G2, int* size_candidates, bool* T2_tilde){
    Node* covered_neighbords = (Node*) malloc(G1->nodes[u].num_neighbors * sizeof(Node));
    int num_nodes = 0;

    for(int i=0; i<G1->nodes[u].num_neighbors; i++){
        int nbr = G1->nodes[u].neighborhood[i];
        int mapping = G1->nodes[nbr].mapped;
        if(mapping != -1){
            covered_neighbords[num_nodes++] = G2->nodes[mapping];
        }
    }

    if(num_nodes == 0){
        int* candidates = (int*)malloc(G2->num_nodes * sizeof(int));
        for(int i=0; i<G2->num_nodes; i++){
            if((G2->nodes[i].mapped == -1) && (T2_tilde[i]==true) && (G2->nodes[i].num_neighbors == G1->nodes[u].num_neighbors))
                candidates[num_nodes++] = i;
        }
        *size_candidates = num_nodes;
        for (int i = 0; i < G1->nodes[u].num_neighbors; i++) {
            covered_neighbords[i].neighborhood = NULL;
        }
        free(covered_neighbords);
        return candidates;
    }

    else{

        if(num_nodes  == 1){
            int* candidates = (int*)malloc(covered_neighbords[0].num_neighbors* sizeof(int));
            int num_candidates = 0;
            *size_candidates = num_candidates;
            int node = G1->nodes[covered_neighbords[0].mapped].mapped;
            for(int i =0; i<covered_neighbords[0].num_neighbors; i++ ){
                int candidate = G2->nodes[node].neighborhood[i];
                if(((G2->nodes[candidate].num_neighbors == G1->nodes[u].num_neighbors) && (G2->nodes[candidate].mapped == -1))){
                    candidates[num_candidates++] = candidate;
                }
            }
            *size_candidates = num_candidates;
            for (int i = 0; i < G1->nodes[u].num_neighbors; i++) {
                covered_neighbords[i].neighborhood = NULL;
            }
            free(covered_neighbords);
            return candidates;
        }
        else{
            int max_dim=0;
            for(int c=0; c<num_nodes; c++){
              max_dim += covered_neighbords[c].num_neighbors;
            }
            int* candidates = (int*)malloc( max_dim* sizeof(int));
            int num_candidates = 0;
            *size_candidates = num_candidates;
            qsort(covered_neighbords, num_nodes, sizeof(Node), comparator);
            bool found = false; int count = 0;
            int size_covered_neighbor_0 = covered_neighbords[0].num_neighbors;
            for(int elem = 0; elem< size_covered_neighbor_0; elem++){
                count=0;
                int node = covered_neighbords[0].neighborhood[elem];
                if(!((G2->nodes[node].num_neighbors == G1->nodes[u].num_neighbors) && (G2->nodes[node].mapped == -1))){
                    continue;
                }

                for(int i=1; i < num_nodes; i++){
                    for(int j=0; j<covered_neighbords[i].num_neighbors; j++){
                        if(node == covered_neighbords[i].neighborhood[j]){
                            found = true;
                            count++;
                            break;
                        }
                    }
                    if(found){
                        found = false;
                        continue;
                    }else{
                        break;
                    }
                }
                if (count == num_nodes -1 ){
                    candidates[num_candidates++] = node;
                    *size_candidates = num_candidates;
                }
            }
            for (int i = 0; i < G1->nodes[u].num_neighbors; i++) {
                covered_neighbords[i].neighborhood = NULL;
            }
            free(covered_neighbords);
            return candidates;
        }

    }

}

/**
 * This function compares two nodes based on their degree (number of neighbors). 
 * It is used by qsort to sort an array of nodes.
 * 
 * 1. It receives two generic pointers a and b representing nodes.
 * 2. It converts these pointers to nodes (Node*) and compares the number of neighbors (num_neighbors).
 * 3. Returns: 
 *    A negative value if node1 has fewer neighbors than node2.
 *    Zero if they have the same number of neighbors.
 *    A positive value if node1 has more neighbors than node2.
 */
int comparator(const void* a, const void* b) {
    const Node* node1 = *(const Node**)a;
    const Node* node2 = *(const Node**)b;

    return node1->num_neighbors - node2->num_neighbors;
}
