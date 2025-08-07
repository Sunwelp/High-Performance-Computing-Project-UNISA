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
 * @file stack.c
 * @brief This file contains the functions useful for managing the stack that stores a node's candidates in order to keep track of mappings.
 * 
 * @dependencies
 * This module depends on the following files:
 * - stack.h: This file defines the data structures and functions to manage the stack that stores a node's candidates in order to keep track of mappings.
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/**
 * Initializes an empty stack with a default minimum capacity (4 elements).
 * Allocates memory for the dynamic array of stack elements and sets the top index to -1, indicating that the stack is initially empty.
 * If the memory allocation fails, an error with perror is shown and the program terminates.
 * The initial capacity can be dynamically increased later.
 */
void init_stack(Stack* stack) {
    stack->capacity = 4;
    stack->elements = malloc(stack->capacity * sizeof(NodeCandidates));
    if (stack->elements == NULL) {
        perror("Stack allocation error");
        exit(EXIT_FAILURE);
    }
    stack->top = -1; 
}

/**
 * Doubles or changes the stack capacity to a new specified size.
 * This is done by resizing the dynamic array elements via realloc.
 * If the resize operation fails, an error is shown and the program terminates.
 * The stack capacity is updated at new_capacity.
 */
void resize_stack(Stack* stack, int new_capacity) {
    NodeCandidates* new_elements = realloc(stack->elements, new_capacity * sizeof(NodeCandidates));
    if (new_elements == NULL) {
        perror("Error during stack resizing");
        exit(EXIT_FAILURE);
    }
    stack->elements = new_elements;
    stack->capacity = new_capacity;
}

/**
 * It reduces the capacity of the stack if the number of elements (top) is less than a quarter of the current capacity and if the capacity is greater than the minimum capacity (4 elements).
 * The function uses resize_stack to reduce the memory used. 
 * It maintains stack efficiency by limiting memory wastage when the number of elements is much less than the allocated capacity.
 */
void shrink_stack(Stack* stack) {
    if (stack->top < stack->capacity / 4 && stack->capacity > 4) {
        resize_stack(stack, stack->capacity / 2);
    }
}

/**
 * Adds a new element of type NodeCandidates to the top of the stack.
 *  If the stack is full, it automatically doubles its capacity using resize_stack.
 *  Allocates memory for the array flags of each NodeCandidates and initializes it to false for each candidate.
 *  Increments the top index to add the new element.
 *  If the memory allocation for flags fails, the program terminates.
 */
void push(Stack* stack, NodeCandidates node) {
    if (stack->top == stack->capacity - 1) {
        resize_stack(stack, stack->capacity * 2);
    }

    node.flags = malloc(node.num_candidates * sizeof(bool));
    if (node.flags == NULL) {
        perror("Allocation error for flags");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < node.num_candidates; i++) {
        node.flags[i] = false;
    }

    stack->top++;
    stack->elements[stack->top] = node; 
}

/**
 * Removes and returns the element at the top of the stack. 
 * Reduces the capacity of the stack using shrink_stack if there are few elements over capacity. 
 * Frees the memory associated with the candidates and flags arrays of the removed node.
 * If the stack is empty (top == -1), shows an error and terminates the program. Returns the removed element, with the freed arrays (arrays are no longer valid).
 */
NodeCandidates pop(Stack* stack) {
    if (stack->top == -1) {
        perror("Stack underflow: stack is empty");
        exit(EXIT_FAILURE);
    }

    NodeCandidates node = stack->elements[stack->top];

    free(node.candidates);
    free(node.flags);

    stack->top--;

    shrink_stack(stack);

    return node; 
}

/**
 * Frees up all memory associated with the stack, including: 
 *   the dynamic candidates and flags arrays of each NodeCandidates in the stack. 
 *   the dynamic elements array that contains the NodeCandidates objects.
 * Iterate on all elements in the stack to free the internal arrays.
 * After liberation, the stack can no longer be used without a new initialization.
 */
void free_stack(Stack* stack) {
    for (int i = 0; i <= stack->top; i++) {
        free(stack->elements[i].candidates); 
        free(stack->elements[i].flags);      
    }
    free(stack->elements);
}

/**
 * Returns the element at the top of the stack without removing it. 
 * If the stack is empty (top == -1), it shows an error and terminates the program.
 * Does not change the stack structure, keeping all elements intact.
 */
NodeCandidates peek(Stack* stack) {
    if (stack->top == -1) {
        perror("Stack underflow: stack is empty");
        exit(EXIT_FAILURE);
    }

    return stack->elements[stack->top]; 
}
