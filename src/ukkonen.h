//
// Created by bert on 12/10/18.
//

#ifndef PROJECT_UKKONEN_H
#define PROJECT_UKKONEN_H

#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ASCII_SIZE 128

typedef struct Edge {
    int id;
    int start;
    int* end;
    struct Edge** children;
    struct Edge* suffix_link;
    uint32_t begin_suffix;
} Edge;

struct ActivePoint {
    Edge* activeNode;
    int activeLength;
    int activeChar;
    int remaining;
};

struct Tree {
    Edge* root;
    Edge* lastSplit;
    char* code;
    int n;
    char c;
    uint32_t beg;
    int STRING_SIZE;
};

struct Tree* build_tree ();

bool add_char(struct Tree* tree, struct ActivePoint* active_point);

void read_char (struct Tree* tree);

void update_active_point(struct ActivePoint* active_point, struct Tree* tree);

void reset_active_edge(struct ActivePoint* active_point, int n);

Edge* create_edge ();

void activate_edge(Edge* e);

void print_and_free(struct Tree* tree);

void depth_first_search (Edge* edge, int* id, int distance, char* code);

bool is_leaf (Edge* edge);