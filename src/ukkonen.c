//
// Created by bert on 12/10/18.
//

#define _GNU_SOURCE
#include <stdio.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <printf.h>
#include <memory.h>
#include "ukkonen.h"


void readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    int n = -1;
    int c;
    struct Edge* root = create_edge();
    activate_edge(root);

    int activeEdge;
    Edge* activeNode = root;
    int activeLength = 0;
    int remainder = 0;

    if (file == NULL) return;
    long f_size = ftell(file);
    code = malloc(f_size);

    c = fgetc(file);
    code[++n] = (char) c;

    Edge* lastSplit = NULL;

    while (c != EOF) {
        remainder++;
        lastSplit = NULL;
        while (remainder > 0) {
           if (activeLength == 0) {
               activeEdge = n;
           }
           if (activeNode->children ==  NULL || activeNode->children[code[activeEdge]] == NULL) {
               Edge* new = create_edge();
               new->start = n;
               new->end = &n;
               if (activeNode->children == NULL) {
                   activate_edge(activeNode);
               }
               activeNode->children[code[activeEdge]] = new;
               if (lastSplit != NULL) {
                   lastSplit->suffix_link = activeNode;
               }
               lastSplit = activeNode;
           } else {
               Edge* next = activeNode->children[code[activeEdge]];
               if (activeLength >= *next->end - next->start + 1) {
                   activeEdge += *next->end - next->start + 1;
                   activeLength -= *next->end - next->start + 1;
                   activeNode = next;
                   continue;
               }
               if (code[next->start + activeLength] == c) {
                   activeLength++;
                   if (lastSplit != NULL) {
                       lastSplit->suffix_link = activeNode;
                   }
                   lastSplit = activeNode;
                   break;
               }
               // Split een edge in de originele en een nieuwe
               Edge* leaf = create_edge();
               leaf->start = n;
               leaf->end = &n;
                // Hier geraken we pas nadat we een karakter lezen die er niet is, vandaar de n

               Edge* split = create_edge();
               activate_edge(split);
               activeNode->children[code[next->start]] = split;

               split->start = next->start;
               int *end = malloc(sizeof(int));
               *end = next->start + activeLength - 1;
               split->end = end;

               split->children[c] = leaf; //nope
               next->start += activeLength;
               split->children[code[next->start]] = next;
               if (lastSplit != NULL) {
                   lastSplit->suffix_link = split;
               }
               lastSplit = split;           }
           remainder --;
           if (activeNode == root && activeLength > 0) {
               activeLength--;
               activeEdge = n - remainder + 1;
           } else {
               if (activeNode->suffix_link != NULL) {
                   activeNode = activeNode->suffix_link;
               } else {
                   activeNode = root;
               }
           }
       }
        c = fgetc(file);
        code[++n] = (char) c;
    }
    --n;
    int* id = malloc(sizeof(int));
    *id = 0;
    depth_first_search(root, id);
    free(id);
    edge_print(root, code, 0);
    printf("%d\n", remainder);
}

Edge* create_edge() {
    Edge* e         = malloc(sizeof(Edge));
    e->suffix_link  = NULL;
    return e;
}

void activate_edge(Edge* e) {
    e->children     = calloc(ASCII_SIZE, sizeof(Edge*));
}

void edge_print(Edge* edge, char* code, int distance) {
    int end;
    if (is_leaf(edge)) {
        if (edge->end == NULL) {
            // edge is de root
            printf("%d @ -\n", edge->id);
        } else {
            printf("%d @ %d-%d\n", edge->id, edge->start - distance , *edge->end);
        }
    } else {
        if (edge->end == NULL) {
            end = 0;
            char* children = get_children(edge, code);
            printf("%d @  -  = %s\n", edge->id, children);
        } else {
            end = *edge->end + 1;
            char* children = get_children(edge, code);
            printf("%d @ %d-%d = %s\n", edge->id, edge->start - distance, *edge->end, children);
        }
        for (int i = 0; i< ASCII_SIZE; i++) {
            if (edge->children[i] != NULL) {
                edge_print(edge->children[i], code, distance + (end - edge->start));
            }
        }
    }
}

bool is_leaf(Edge* edge) {
    return edge->children == NULL;
}

void depth_first_search(Edge* edge, int* id) {
    if (edge->children != NULL) {
        for (int i = 0; i< ASCII_SIZE; i++) {
            if (edge->children[i] != NULL) {
                edge->children[i]->id = ++*id;
                depth_first_search(edge->children[i], id);
            }
        }
    }
}

char* get_children(Edge* edge, char* code) {
    char* children = "";
    for (int i = 0; i< ASCII_SIZE; i++) {
        if (edge->children[i] != NULL) {
            if (strcmp(children, "") == 0) {
                asprintf(&children, "%s%d:%d,%d-%d", children, i, edge->children[i]->id, edge->children[i]->start, *edge->children[i]->end );
            } else {
                asprintf(&children, "%s | %d:%d,%d-%d", children, i, edge->children[i]->id, edge->children[i]->start, *edge->children[i]->end );
            }
        }
    }
    return children;
}
