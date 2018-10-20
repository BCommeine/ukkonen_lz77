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

    Edge* activeEdge;
    Edge* activeNode = root;
    int activeLength = 0;
    int remainder = 1;

    if (file == NULL) return;
    long f_size = ftell(file);
    code = malloc(f_size);

    c = fgetc(file);
    code[++n] = (char) c;

    Edge* lastSplit = NULL;

    while (c != EOF) {
        // Check if we're dealing with duplicates
        if (activeNode == root && activeLength == 0) {
            if (root->children[c] == NULL) {
                struct Edge* new = create_edge();
                new->start = n;
                new->end = &n;
                root->children[c] = new;
                c = fgetc(file);
                code[++n] = (char) c;
                lastSplit = NULL;
            } else {
                activeEdge = root->children[c];
                activeLength = 1;
                remainder++;
                c = fgetc(file);
                code[++n] = (char) c;
                lastSplit = NULL;
            }
        } else {
            if (activeLength - 1 < *activeEdge->end - activeEdge->start) {
                // Er zijn nog characters op de huidige edge
                if (code[activeEdge->start + activeLength] == c) {
                    // Het volgend karakter zit al in de trie
                    activeLength++;
                    remainder++;
                    c = fgetc(file);
                    code[++n] = (char) c;
                    lastSplit = NULL;
                } else {
                    // Split een edge in de originele en een nieuwe
                    struct Edge *newEdge = create_edge();
                    newEdge->start = n;
                    newEdge->end = &n;
                    struct Edge *splitEdge = create_edge();

                    // SWAP
                    Edge* tmp = splitEdge->children;
                    splitEdge->children = activeEdge->children;
                    activeEdge->children = tmp;

                    splitEdge->start = activeEdge->start + activeLength;
                    splitEdge->end = activeEdge->end;
                    activeEdge->children[code[activeEdge->start + activeLength]] = splitEdge;
                    activeEdge->children[c] = newEdge;
                    int* end = malloc(sizeof(int));
                    *end = activeEdge->start + activeLength - 1;
                    activeEdge->end = end;
                    remainder--;
                    if (lastSplit != NULL) {
                        lastSplit->suffix_link = activeEdge;
                    }
                    lastSplit = activeEdge;
                    if (activeNode == root) {
                        activeEdge = root->children[code[n - --activeLength]];
                    } else {
                        if (activeNode->suffix_link != NULL) {
                            activeNode = activeNode->suffix_link;
                            activeEdge = activeNode->children[code[n - activeLength]];
                        } else {
                            activeNode = root;
                            activeEdge = activeNode->children[code[n - activeLength]];
                        }
                    }
                }
            } else {
                // Vorige c is het laatste karakter op de edge
                if (activeEdge->children[c] == NULL) {
                    struct Edge *new = create_edge();
                    new->start = n;
                    new->end = &n;
                    activeEdge->children[c] = new;
                    remainder--;
                } else {
                    activeNode = activeEdge;
                    activeEdge = activeEdge->children[c];
                    activeLength = 1; // Moet dit niet 0 zijn?
                    remainder++;
                    c = fgetc(file);
                    code[++n] = (char) c;
                    lastSplit = NULL;
                }
            }
        }
    }
    --n;
    int* id = malloc(sizeof(int));
    *id = 0;
    depth_first_search(root, id);
    free(id);
    edge_print(root, code);
}

Edge* create_edge() {
    Edge* e         = malloc(sizeof(Edge));
    e->children     = calloc(256, sizeof(Edge*));
    e->suffix_link  = NULL;
    return e;
}

void edge_print(Edge* edge, char* code) {
    for (int i = 0; i< 256; i++) {
        if (edge->children[i] != NULL){
            if (is_leaf(edge->children[i])){
                printf("%d @ %d-%d\n", edge->children[i]->id, edge->children[i]->start, *edge->children[i]->end);
                edge_print(edge->children[i], code);
            } else {
                char* children = get_children(edge->children[i], code);
                printf("%d @ %d-%d = %s\n", edge->children[i]->id, edge->children[i]->start, *edge->children[i]->end, children);
                edge_print(edge->children[i], code);
            }
        }
    }
}

bool is_leaf(Edge* edge) {
    bool leaf = true;
    int index = 0;
    while (leaf && index < 256) {
        if (edge->children[index] != NULL) {
            leaf = false;
        }
        index++;
    }
    return leaf;
}

void depth_first_search(Edge* edge, int* id) {
    int index = 0;
    while (index < 256) {
        if (edge->children[index] != NULL) {
            edge->children[index]->id = ++*id;
            depth_first_search(edge->children[index], id);
        }
        index++;
    }
}

char* get_children(Edge* edge, char* code) {
    char* children = "";
    int index = 0;
    while (index < 256) {
        if (edge->children[index] != NULL) {
            if (children == "") {
                asprintf(&children, "%s%d:%d,%d-%d", children, index, edge->children[index]->id, edge->children[index]->start, *edge->children[index]->end );
            } else {
                asprintf(&children, "%s | %d:%d,%d-%d", children, index, edge->children[index]->id, edge->children[index]->start, *edge->children[index]->end );
            }
        }
        index++;
    }
    return children;
}