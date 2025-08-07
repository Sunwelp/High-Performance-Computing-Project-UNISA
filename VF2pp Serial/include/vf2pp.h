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
 * @file vf2pp.h
 * @brief This file defines the main functions used to check whether two graphs are isomorphic.
 * 
 * @dependencies
 * This module depends on the following files:
 * - graph.h: Definition of graph structure.
 */

#ifndef VF2PP_H
#define VF2PP_H

#include <stdbool.h>
#include "graph.h"

/**
 * @brief This function checks whether two graphs are isomorphic.
 * 
 * @param G1 Pointer to G1 graph.
 * @param G2 Pointer to G2 graph.
 * @return A boolean value indicating whether G1 and G2 are isomorphic.
 */
bool vf2pp_is_isomorphic(Graph* G1, Graph* G2);

/**
 * @brief This function checks whether two graphs are isomorphic.
 * 
 * @param G2 Pointer to G2 graph.
 * @param popped_node2 The node removed from the mapping.
 * @param T2_tilde Boolean array indicating which nodes in `G2` are unmapped and are not neighbors of mapped nodes.
 */
void _restore_Tinout(Graph* G2, int popped_node2, bool* T2_tilde);

/**
 * @brief This function checks whether two graphs are isomorphic.
 * 
 * @param G2 Pointer to G2 graph.
 * @param new_node2 The node added to the mapping.
 * @param T2_tilde Boolean array indicating which nodes in `G2` are unmapped and are not neighbors of mapped nodes.
 */
void _update_Tinout(Graph* G2, int new_node2, bool* T2_tilde);

#endif // VF2PP_H
