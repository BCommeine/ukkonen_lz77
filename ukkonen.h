//
// Created by bert on 12/10/18.
//

#ifndef PROJECT_UKKONEN_H
#define PROJECT_UKKONEN_H

#endif

typedef struct Edge {
    int start;
    int* end;
    struct Edge* children[256];
} Edge;

void readFile (char*);