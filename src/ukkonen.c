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
            if (activeEdge == NULL) {
                struct Edge* new = create_edge();
                new->start = n;
                new->end = &n;
                activeNode->children[c] = new;
                if (activeNode->suffix_link != NULL) {
                    activeNode = activeNode->suffix_link;
                    activeEdge = activeNode->children[code[n - activeLength]];
                } else {
                    activeNode = root;
                    activeEdge = activeNode->children[code[n - activeLength]];
                }
                lastSplit = NULL;
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
                        Edge **tmp = splitEdge->children;
                        splitEdge->children = activeEdge->children;
                        activeEdge->children = tmp;

                        splitEdge->start = activeEdge->start + activeLength;
                        splitEdge->end = activeEdge->end;
                        if (activeEdge->children == NULL) {
                            activate_edge(activeEdge);
                        }
                        activeEdge->children[code[activeEdge->start + activeLength]] = splitEdge;
                        activeEdge->children[c] = newEdge;
                        int *end = malloc(sizeof(int));
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
                        if (lastSplit != NULL) {
                            lastSplit->suffix_link = activeEdge;
                        }
                        activeEdge->children[c] = new;
                        remainder--;
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
                    } else {
                        activeNode = activeEdge;
                        activeEdge = activeEdge->children[c];
                        activeLength = 1;
                        remainder++;
                        c = fgetc(file);
                        code[++n] = (char) c;
                        lastSplit = NULL;
                    }
                }
            }
        }
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
