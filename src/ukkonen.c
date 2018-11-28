//
// Created by bert on 12/10/18.
//

#define _GNU_SOURCE

#include <string.h>
#include "ukkonen.h"

struct Tree* build_tree() {
    struct ActivePoint active_point;
    struct Tree* tree = malloc(sizeof(struct Tree));

    tree->root = create_edge();
    tree->root->begin_suffix = 0;
    tree->STRING_SIZE = 1024;

    // ACTIVE POINT
    active_point.activeChar = 0;
    active_point.activeNode = tree->root;
    active_point.activeLength = 0;

    tree->code = malloc(tree->STRING_SIZE * sizeof(char));
    tree->n = -1;
    read_char(tree);

    active_point.remaining = 0;
    tree->lastSplit= NULL;

    while (tree->c != EOF) {
        active_point.remaining++;
        tree->lastSplit = NULL;
        reset_active_edge(&active_point, tree->n);
        add_char(tree, &active_point);
        read_char(tree);
    }
    // We hebben zonet het EOF karakter toegevoegd aan onze string. Nu zetten ze onze index een stap terug,
    // en zetten het laatste karakter op een null termination
    tree->code[tree->n--] = '\0';
    return tree;
}

bool add_char(struct Tree* tree, struct ActivePoint* active_point) {
    while (active_point->remaining > 0) {
        if (active_point->activeNode->children == NULL) {
            activate_edge(active_point->activeNode);
        }
        if (active_point->activeNode->children[(unsigned char) tree->code[active_point->activeChar]] == NULL) {
            Edge* new = create_edge();
            new->start = tree->n;
            new->end = &tree->n;
            active_point->activeNode->children[(unsigned char) tree->code[active_point->activeChar]] = new;
            if ( active_point->activeNode != tree->root) {
                if (tree->lastSplit != NULL) {
                    tree->lastSplit->suffix_link = active_point->activeNode;
                }
                tree->lastSplit = active_point->activeNode;
            }
            new->begin_suffix = tree->n - (active_point->activeNode->start - active_point->activeNode->begin_suffix);
            active_point->remaining--;
            update_active_point(active_point, tree);
        } else {
            Edge *activeEdge = active_point->activeNode->children[(unsigned char) tree->code[active_point->activeChar]];
            int length = *activeEdge->end - activeEdge->start + 1;
            if (active_point->activeLength < length) {
                if (tree->code[activeEdge->start + active_point->activeLength] == tree->c) {
                    bool result;
                    active_point->activeLength++;
                    if (tree->lastSplit != NULL) {
                        result = false;
                        tree->lastSplit->suffix_link = active_point->activeNode;
                    } else {
                        result = true;
                        tree->beg = activeEdge->begin_suffix;
                    }
                    tree->lastSplit = active_point->activeNode;
                    return false;
                }
                Edge *leaf = create_edge();
                leaf->start = tree->n;
                leaf->end = &tree->n;

                Edge *split = create_edge();
                activate_edge(split);
                active_point->activeNode->children[(unsigned char) tree->code[activeEdge->start]] = split;

                split->start = activeEdge->start;
                split->begin_suffix = activeEdge->begin_suffix;

                int *end = malloc(sizeof(int));
                *end = activeEdge->start + active_point->activeLength - 1;
                split->end = end;

                split->children[ (unsigned char) tree->c] = leaf; //nope
                activeEdge->start += active_point->activeLength;
                split->children[ (unsigned char) tree->code[activeEdge->start]] = activeEdge;
                if (tree->lastSplit != NULL) {
                    tree->lastSplit->suffix_link = split;
                }
                tree->lastSplit = split;
                leaf->begin_suffix = tree->n - (activeEdge->start - activeEdge->begin_suffix); // start - distance
                active_point->remaining--;
                update_active_point(active_point, tree);
            } else { // SKIP
                active_point->activeChar += length;
                active_point->activeLength -= length;
                active_point->activeNode = activeEdge;
            }
        }
    }
    return false;
}

void read_char(struct Tree* tree){
  if (++tree->n  == tree->STRING_SIZE - 1){
        tree->STRING_SIZE *= 2;
        tree->code = realloc(tree->code, tree->STRING_SIZE * sizeof(char));
    }
    tree->c = (char) fgetc(stdin);
    tree->code[tree->n] = tree->c;
}


void update_active_point(struct ActivePoint* active_point, struct Tree* tree){
    if (active_point->activeNode == tree->root && active_point->activeLength > 0) {
        active_point->activeLength--;
        active_point->activeChar = tree->n - active_point->remaining + 1;
    } else {
        if (active_point->activeNode != tree->root && active_point->activeNode->suffix_link != NULL) {
            active_point->activeNode = active_point->activeNode->suffix_link;
        } else {
            active_point->activeNode = tree->root;
        }
    }
}

void reset_active_edge(struct ActivePoint* active_point, int n) {
    if (active_point->activeLength == 0) {
        active_point->activeChar = n;
    }
}

Edge* create_edge() {
    return calloc(1, sizeof(Edge));
}

void activate_edge(Edge* e) {
    e->children     = calloc(ASCII_SIZE, sizeof(Edge*));
}

bool is_leaf(Edge* edge) {
    return edge->children == NULL;
}

void print_and_free(struct Tree* tree) {
    int* id = malloc(sizeof(int));
    *id = -1;
    depth_first_search(tree->root, id, 0, tree->code);
    free(id);
    free(tree->root);
    free(tree->code);
    free(tree);
}

void depth_first_search(Edge* edge, int* id, int distance, char* code) {
    edge->id = ++*id;
    if (!is_leaf(edge)) {
        int end = edge->end == NULL? 0: *edge->end + 1;
        char* children = "";
        char* tmp = NULL;
        for (int i = 0; i < ASCII_SIZE; i++) {
            if (edge->children[i] != NULL) {
                depth_first_search(edge->children[i], id, distance + (end - edge->start), code);
                if (tmp == NULL) {
                    asprintf(&children, "%d:%d,%d-%d", i, edge->children[i]->id, edge->children[i]->start,
                             *edge->children[i]->end);
                } else {
                    asprintf(&children, "%s | %d:%d,%d-%d", tmp, i, edge->children[i]->id,
                             edge->children[i]->start, *edge->children[i]->end);
                }
                free(tmp);
                tmp = children;
                if (!is_leaf(edge->children[i])) {
                    free(edge->children[i]->end);
                }
                free(edge->children[i]);
            }
        }
        free(edge->children);
        if (edge->end == NULL) {
            printf("%d @  -  = %s\n", edge->id, children);
        } else {
            printf("%d @ %d-%d = %s\n", edge->id, edge->start - distance, *edge->end, children);
        }
        free(children);
    } else {
        if (edge->end == NULL) {
            // edge is de root
            printf("%d @ -\n", edge->id);
        } else {
            printf("%d @ %d-%d\n", edge->id, edge->start - distance, *edge->end);
        }
    }
}
