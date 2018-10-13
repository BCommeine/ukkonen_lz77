//
// Created by bert on 12/10/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "ukkonen.h"

void readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    int n = -1;
    int c;
    struct Edge* root = malloc (sizeof(struct Edge));

    Edge* activeEdge;
    Edge* activeNode = root;
    int activeLength = 0;
    int remainder = 1;

    if (file == NULL) return;
    long f_size = ftell(file);
    code = malloc(f_size);

    while (c != EOF) {
        Edge* lastSplit;
        // Check if we're dealing with duplicates
        if (activeNode == root && activeLength == 0) {
            if (root->children[c] == NULL) {
                struct Edge* new = malloc (sizeof(struct Edge));
                new->start = n;
                new->end = &n;
                root->children[c] = new;
                c = fgetc(file);
                code[++n] = (char) c;
                Edge* lastSplit = NULL;

            } else {
                activeEdge = root->children[c];
                activeLength = 1;
                remainder++;
                c = fgetc(file);
                code[++n] = (char) c;
                Edge* lastSplit = NULL;
            }
        } else {
            if (activeLength < *activeEdge->end - activeEdge->start) {
                // Er zijn nog characters op de huidige edge
                if (code[activeEdge->start + activeLength] == c) {
                    // Het volgend karakter zit al in de trie
                    activeLength++;
                    remainder++;
                    c = fgetc(file);
                    code[++n] = (char) c;
                    Edge* lastSplit = NULL;
                } else {
                    // Split een edge in de originele en een nieuwe
                    struct Edge *newEdge = malloc(sizeof(struct Edge));
                    newEdge->start = n;
                    newEdge->end = &n;
                    activeEdge->children[c] = newEdge;
                    struct Edge *splitEdge = malloc(sizeof(struct Edge));
                    splitEdge->start = activeEdge->start + activeLength;
                    splitEdge->end = activeEdge->end;
                    activeEdge->children[code[activeEdge->start + activeLength]] = splitEdge;
                    int end = malloc(sizeof(int));
                    end = activeEdge->start + activeLength;
                    activeEdge->end = &end;
                    remainder--;
                    if (lastSplit != NULL) {
                        activeEdge->suffix_link = malloc(sizeof(struct Edge));
                        activeEdge->suffix_link = lastSplit;
                    }
                    lastSplit = activeEdge;
                    if (activeNode == root) {
                        activeEdge = root->children[code[--activeLength]];
                    }
                }
            } else {
                // Vorige c is het laatste karakter op de edge
                if (activeEdge->children[c] == NULL) {
                    struct Edge *new = malloc(sizeof(struct Edge));
                    new->start = n;
                    new->end = &n;
                    activeEdge->children[c] = new;
                    remainder--;
                } else {
                    activeNode = activeEdge;
                    activeEdge = activeEdge->children[c];
                    activeLength = 1;
                    remainder++;
                    c = fgetc(file);
                    code[++n] = (char) c;
                    Edge* lastSplit = NULL;
                }
            }
        }
      }
}