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
 * @file find_candidates.h
 * @brief This file defines functions to find nodes of G2 that are candidates to be mapped to a node of G1
 * 
 * @dependencies
 * This module depends on the following files:
 * - graph.h: Definition of graph structure.
 */
#ifndef FIND_CANDIDATES_H
#define FIND_CANDIDATES_H

#include <stdbool.h>
#include "graph.h"

/**
 * @brief This function finds candidates for the `u` node of a graph.
 *
 * @param u Index of the node in the `G1` graph.
 * @param G1 Pointer to G1 graph.
 * @param G2 Pointer to G2 graph.
 * @param size_candidates Pointer to an integer representing the number of candidates found.
 * @param T2_tilde Boolean array indicating which nodes in `G2` are unmapped and are not neighbors of mapped nodes.
 * @return Pointer to an array containing the indexes of candidate nodes.
 */
int* _find_candidates(int u, Graph* G1, Graph* G2, int* size_candidates, bool* T2_tilde);

/**
 * @brief Comparison function for sorting. Compares two nodes by the number of neighbors. Used in `qsort`.
 *
 * @param a Pointer to the first node.
 * @param b Pointer to the second node.
 * @return A negative value if the first node has fewer neighbors, positive if it has more, 0 if they are equal.
 */
int comparator(const void* a, const void* b);

#endif // FIND_CANDIDATES_H
