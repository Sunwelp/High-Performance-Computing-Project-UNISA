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
 * @file stack.h
 * @brief This file defines the data structures and functions to manage the stack that stores a node's candidates in order to keep track of mappings .
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @struct NodeCandidates
 * @brief That structure represents a node and its candidates.
 *
 * @field node The index of the current node extracted from the node order.
 * @field candidates A dynamic array containing the indexes of candidate nodes.
 * @field flags A dynamic array of Boolean flags, one for each candidate, to indicate the status (validity) of the candidate.
 * @field num_candidates The total number of valid candidates associated with the node.
 */
typedef struct {
    int node;            
    int* candidates;    
    bool* flags;         
    int num_candidates;  
} NodeCandidates;

/**
 * @struct Stack
 * @brief This structure implements a dynamic stack to store elements of type NodeCandidates.
 *
 * @field elements A dynamic array containing objects of type NodeCandidates.
 * @field top The index of the last element in the stack. It is -1 if the stack is empty.
 * @field capacity The current maximum capacity of the stack.
 */
typedef struct {
    NodeCandidates* elements; 
    int top;                 
    int capacity;            
} Stack;

/**
 * @brief This function initializes a dynamic stack to store objects of type NodeCandidates.
 *
 * @param stack Pointer to the stack to be initialized.
 */
void init_stack(Stack* stack);

/**
 * @brief This function dynamically resizes the stack to increase its capacity.
 *
 * @param stack Pointer to the stack to be resized.
 * @param new_capacity The new stack capacity.
 */
void resize_stack(Stack* stack, int new_capacity);

/**
 * @brief This function dynamically reduces the stack capacity to free unused memory.
 *
 * @param stack Pointer to the stack to be reduced.
 */
void shrink_stack(Stack* stack);

/**
 * @brief This function adds a new element to the top of the stack.
 *
 * @param stack Pointer to the stack on which to perform the operation.
 * @param node The element of type NodeCandidates to be added.
 */
void push(Stack* stack, NodeCandidates node);

/**
 * @brief This function removes and returns the element at the top of the stack.
 *
 * @param stack Pointer to the stack from which to extract the element.
 * @return The element of type NodeCandidates at the top of the stack.
 */
NodeCandidates pop(Stack* stack);

/**
 * @brief This function returns the element at the top of the stack without removing it.
 * 
 * @param stack Pointer to the stack from which to obtain the element.
 * @return The element of type NodeCandidates at the top of the stack.
 */
NodeCandidates peek(Stack* stack);

/**
 * @brief Frees up memory allocated for the stack and its elements.
 *
 * @param stack Pointer to the stack to be freed.
 */
void free_stack(Stack* stack);