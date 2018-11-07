//
// Created by bert on 12/10/18.
//

#ifndef PROJECT_UKKONEN_H
#define PROJECT_UKKONEN_H

#endif

#include <stdbool.h>
#include <stdio.h>

#define ASCII_SIZE 128

typedef struct Edge {
    int id;
    int start;
    int* end;
    struct Edge** children;
    struct Edge* suffix_link;
} Edge;

void readFile (char*);

Edge* create_edge ();

void depth_first_search (Edge* edge, int* id);

bool is_leaf (Edge* edge);

char* get_children (Edge* edge, char* code);

void edge_print(Edge* edge, char* code, int distance);