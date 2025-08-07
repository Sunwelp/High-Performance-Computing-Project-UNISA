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
/* Worst case line length for a fully connected graph of 1000 nodes,
 * parsed in the file with TAB and SPACE characters
 * To compute a new value use:
 *      (MAX_NODE_LENGTH + 1) + (MAX_NODE_LENGTH * NODES) + 1
 * with MAX_NODE_LENGTH being the number of characters used to represent the last
 * node; for example, with 1000 nodes we compute:
 *       (4 + 1) + (4 * 1000) = 5 + 4000 + 1 = 4006
 * (in this case MAX_NODE_LENGTH is 4)
 */
#define MAX_LINE_LENGTH 4006

/* All limits for arrays and values */
#define MAX_FL_LENGTH 10
#define MAX_NEIGHBOUR_LIST 1000
#define INT_MIN 0
#define INT_MAX 1000

#include "graph.h"
#include "node_ordering.h"
#include "errno.h"
#include "limits.h"
#include "ctype.h"

/**
 *
 *  @details Dynamically create the graph structure:
 *           using malloc() assign memory area to the graph structure
 *           and to all the nodes; then set the nodes parameters to
 *           the initial value.
 */
Graph* createGraph(int num_nodes){
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->nodes = (Node*)malloc(num_nodes * sizeof(Node));
    g->num_nodes = num_nodes;

    for(int i = 0; i < num_nodes; i++){
        g->nodes[i].neighborhood = NULL;
        g->nodes[i].num_neighbors = 0;
        g->nodes[i].ID = i;
        g->nodes[i].mapped = -1;
    }
    return g;
}

/**
 *
 *  @details Free the memory area assigned to a graph
 *           deleting all the neighbours lists, nodes and
 *           graph structures itself.
 */
void freeGraph(Graph* g){

    for(int i = 0; i< g->num_nodes; i++){
        free(g->nodes[i].neighborhood);
    }
    free(g->nodes);
    free(g);
}

/**
 *
 *  @details Update the node structures of the graph:
 *           for each node create and fill it's neighbour
 *           list
 */
void addEdges(Graph* g, int node, int* list, int neighbours){

    /* Check for incompatible node */
    if(node > g->num_nodes){
        fprintf(stderr, "ERROR: startNode out of bounds.");
        return;
    }

    /* Dynamically change the size of the neighbour list, to match the actual list size*/
    Node* Node = &g->nodes[node];
    int* tempNodeNL = (int*)realloc(Node->neighborhood, (neighbours)*sizeof(int));
    if(tempNodeNL == NULL){
        fprintf(stderr, "ERROR: memory not reallocated for node %d", node);
        exit(EXIT_FAILURE);
    }
    Node->neighborhood = tempNodeNL;
    Node->num_neighbors = neighbours;

    /* Add all neighbour to the node's neighbour list*/
    for(int i = 0; i < neighbours; i++){
        Node->neighborhood[i] = list[i];
    }
}

/**
 *  @details Read a graph from a text file, with a specific format.
 *
 *           File format must be as follow:
 *              a) first line must contains the total number of nodes
 *                 of the graph and the total number of lines in the file,
 *                 separated by a single space;
 *              b) each subsequent line contains the nodes and it's neighbours list,
 *                 formatted as follow:
 *
 *                 node    neighbour1 neighbour2 neighbour3...etc
 *
 *            The node and neighbours list are separated by a \t
 *            while each neighbour in the list is separated by a single space.
 *
 *            The function call first the "createGraph()" function and then,
 *            for each node, the "addEdges()" function, both described above.
 *
 */
Graph* readGraphFromFile(const char *filename, bool VERBOSE){

    if(!filename){
      fprintf(stderr, "ERROR: Filename is NULL.");
      return NULL;
    }
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file");
        return NULL;
    }

    char firstLine[MAX_FL_LENGTH];
    int numNodes = 0, numLines = 0;

    /* Get first line parameters: num nodes and total number of file's lines */
    if (fgets(firstLine, MAX_FL_LENGTH, file)) {
        sscanf(firstLine, "%d %d", &numNodes, &numLines);
        if(VERBOSE == 1) {
            fprintf(stderr, "Number of nodes: %d, Number of lines: %d\n", numNodes, numLines);
        }
    } else {
        fprintf(stderr, "Failed to read the first line.\n");
        fclose(file);
        return NULL;
    }

    Graph* g = createGraph(numNodes);

    char line[MAX_LINE_LENGTH];
    int nLine = 0;
    while(fgets(line, MAX_LINE_LENGTH, file)){
        process_line(g, line, nLine == 0);
        nLine++;
    }

    fclose(file);

    if(VERBOSE){
        fprintf(stderr, "Graph created with %d nodes.\n", g->num_nodes);
        printGraph(g);
        fprintf(stderr, "#######################################\n\n");
    }

    return g;
}

/**
 *
 * @details Process the line passed, separating node ID and neighbour list
 */
void process_line(Graph* g, char *line, int isFirstLine){

    int node;
    char *remainingString;

    /* Replace newline (\n) character, if present, with null terminator (\0) */
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }

    /* Find the tab separator */
    remainingString = strchr(line, '\t');
    if (remainingString == NULL) {
        if(isFirstLine){
            fprintf(stderr, "WARNING: Invalid format, tab not found: skipping first line.\n");
        }
        return;
    }

    /*  Replace tab with null terminator to extract first
     *  number and move past the tab character
     */
    *remainingString = '\0';
    remainingString++;

    /* Read the first number */
    sscanf(line, "%d", &node);

    /* Parse the remaining numbers */
    int adjList[MAX_NEIGHBOUR_LIST];
    int neighbours = 0;
    char *token = strtok(remainingString, " ");

    /* Safely convert char to int, handling errors if any*/
    while (token != NULL && neighbours < MAX_NEIGHBOUR_LIST) {
        char *endptr;
        errno = 0;
        
        while(isspace((unsigned char)*token)) token++;
        if(*token == '\0'){
            token = strtok(NULL, " ");
            continue;
        }
        
        long val = strtol(token, &endptr, 10);

        /* Check for conversion errors and invalid characters */
        if (errno == ERANGE) {
            fprintf(stderr, "ERROR: Number out of range [%ld, %ld]: %s\n", LONG_MIN, LONG_MAX, token);
        } else if (endptr == token) {
            fprintf(stderr, "ERROR: No digit found in token: %s\n", token);
        }else if(*endptr != '\0' && !isspace((unsigned char)*endptr)){
            fprintf(stderr, "ERROR: Trailing non-numeric characters in '%s' (first invalid: '%c')\n", token, *endptr);
        }else if(val < INT_MIN || val > INT_MAX){
            fprintf(stderr, "ERROR: Value %ld exceeds int range [%d, %d].\n", val, INT_MIN, INT_MAX);
        }else {
            adjList[neighbours++] = (int)val;
        }

        token = strtok(NULL, " ");
    }

    addEdges(g, node,adjList, neighbours);
}

/**
*   @details Iterate over each node of the graph and print the node itself
*            and its neighbour list
*/
void printGraph(Graph* g){
    for(int i = 0; i < g->num_nodes; i++){
        fprintf(stderr, "Node: %d -- ", g->nodes[i].ID);
        fprintf(stderr, "Neighbour List: ");
        for (int j = 0; j < g->nodes[i].num_neighbors; j++) {
            fprintf(stderr, " %d", g->nodes[i].neighborhood[j]);
        }
        fprintf(stderr, "\n");
    }
}

/************* FLAT GRAPH FUNCTIONS ************/

/**
 *  @details
 * 
 */
int* flatten_graph(Graph* g, int* flat_size){

    int total_neighbors = 0;
    for (int i = 0; i < g->num_nodes; i++) {
        total_neighbors += g->nodes[i].num_neighbors;
    }

    *flat_size = 1 + g->num_nodes * 3 + total_neighbors;

    int* flat = (int*)malloc((*flat_size) * sizeof(int));
    int pos = 0;

    // Store number of nodes
    flat[pos++] = g->num_nodes;

    for (int i = 0; i < g->num_nodes; i++) {
        Node* n = &g->nodes[i];
        flat[pos++] = n->ID;
        flat[pos++] = n->mapped;
        flat[pos++] = n->num_neighbors;

        for (int j = 0; j < n->num_neighbors; j++) {
            flat[pos++] = n->neighborhood[j];
        }
    }

    return flat;
}


/**
 *  @details
 * 
 */
Graph* unflatten_graph(int* flat){
    int pos = 0;

    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->num_nodes = flat[pos++];
    g->nodes = (Node*)malloc(g->num_nodes * sizeof(Node));

    for (int i = 0; i < g->num_nodes; i++) {
        Node* n = &g->nodes[i];

        n->ID = flat[pos++];
        n->mapped = flat[pos++];
        n->num_neighbors = flat[pos++];

        n->neighborhood = (int*)malloc(n->num_neighbors * sizeof(int));
        for (int j = 0; j < n->num_neighbors; j++) {
            n->neighborhood[j] = flat[pos++];
        }
    }

    free(flat);

    return g;
}